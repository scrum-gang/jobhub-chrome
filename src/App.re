type state = {
  id: option(string),
  token: option(string),
};

type action =
  | DidMount(option(string), option(string))
  | Login(option(string), option(string))
  | Logout;

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  reducer: (action, _state) =>
    switch (action) {
    | DidMount(maybeId, maybeJwt) =>
      ReasonReact.Update({id: maybeId, token: maybeJwt})
    | Login(maybeId, maybeJwt) =>
      ReasonReact.UpdateWithSideEffects(
        {id: maybeId, token: maybeJwt},
        (
          _self => {
            SyncStorage.refreshId(maybeId);
            SyncStorage.refreshToken(maybeJwt);
          }
        ),
      )
    | Logout =>
      ReasonReact.UpdateWithSideEffects(
        {id: None, token: None},
        (_self => SyncStorage.clear()),
      )
    },
  initialState: () => {id: None, token: None},
  didMount: self => {
    let handleExpiredToken = () => self.send(Logout);
    let handleRetrievedId = (maybeId, maybeJwt) =>
      DidMount(maybeId, maybeJwt) |> self.send;
    let handleValidToken =
      fun
      | Some(jwt) =>
        Js.Promise.(
          SyncStorage.getSavedId()
          |> then_(maybeId =>
               switch (maybeId) {
               | Some(id) =>
                 handleRetrievedId(Js.Option.some(id), Js.Option.some(jwt));
                 resolve();
               | None =>
                 handleExpiredToken();
                 resolve();
               }
             )
          |> catch(err => {
               Js.log(err);
               handleExpiredToken();
               resolve();
             })
        )
        |> ignore
      | None => handleExpiredToken();
    Js.Promise.(
      SyncStorage.getSavedToken()
      |> then_(maybeJwt => {
           switch (maybeJwt) {
           | Some(jwt) =>
             Services.validateToken(~jwt, ~callback=handleValidToken)
           | None => handleExpiredToken()
           };
           resolve();
         })
      |> catch(err => {
           Js.log(err);
           handleExpiredToken();
           resolve();
         })
    )
    |> ignore;
    ReasonReact.NoUpdate;
  },
  render: self =>
    <div className="app">
      (
        switch (self.state.token) {
        | None =>
          <Login
            updateAuth=(
              (maybeId, maybeToken) =>
                Login(maybeId, maybeToken) |> self.send
            )
          />
        | Some(token) =>
          <JobApp
            signOutHandler=(_event => self.send(Logout))
            id=(Belt.Option.getWithDefault(self.state.id, ""))
            jwt=token
          />
        }
      )
    </div>,
};
