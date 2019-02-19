type state = {token: option(string)};

type action =
  | DidMount(option(string))
  | Login(option(string))
  | Logout;

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  reducer: (action, _state) =>
    switch (action) {
    | DidMount(maybeJwt) => ReasonReact.Update({token: maybeJwt})
    | Login(maybeJwt) =>
      ReasonReact.UpdateWithSideEffects(
        {token: maybeJwt},
        (_self => SyncStorage.refreshToken(maybeJwt)),
      )
    | Logout =>
      ReasonReact.UpdateWithSideEffects(
        {token: None},
        (_self => SyncStorage.clear()),
      )
    },
  initialState: () => {token: None},
  didMount: self => {
    let handleRetrievedToken = maybeJwt => DidMount(maybeJwt) |> self.send;
    Js.Promise.(
      SyncStorage.getSavedToken()
      |> then_(maybeJwt => {
           handleRetrievedToken(maybeJwt);
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
          <Login updateToken=(maybeToken => Login(maybeToken) |> self.send) />
        | Some(_token) =>
          <JobApp
            submitHandler=(_event => self.send(Logout))
            signOutHandler=(_event => self.send(Logout))
          />
        }
      )
    </div>,
};
