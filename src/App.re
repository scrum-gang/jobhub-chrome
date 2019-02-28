type state = {
  validatedToken: bool,
  token: option(string),
};

type action =
  | DidMount(option(string))
  | Login(option(string))
  | Logout;

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  reducer: (action, _state) =>
    switch (action) {
    | DidMount(maybeJwt) =>
      ReasonReact.Update({validatedToken: true, token: maybeJwt})
    | Login(maybeJwt) =>
      ReasonReact.UpdateWithSideEffects(
        {..._state, token: maybeJwt},
        (_self => SyncStorage.refreshToken(maybeJwt)),
      )
    | Logout =>
      ReasonReact.UpdateWithSideEffects(
        {..._state, token: None},
        (_self => SyncStorage.clear()),
      )
    },
  initialState: () => {validatedToken: false, token: None},
  didMount: self => {
    let handleRetrievedToken = maybeJwt => DidMount(maybeJwt) |> self.send;
    Js.Promise.(
      SyncStorage.getSavedToken()
      |> then_(maybeJwt => {
           switch (maybeJwt) {
           | Some(jwt) =>
             Services.validateToken(
               ~jwt, ~callback=handleRetrievedToken, ~failure=x =>
               x |> Js.log
             )
           | None => handleRetrievedToken(maybeJwt)
           };
           resolve();
         })
    )
    |> ignore;
    ReasonReact.NoUpdate;
  },
  render: self =>
    <div className="app">
      (
        self.state.validatedToken ?
          switch (self.state.token) {
          | None =>
            <Login
              updateToken=(maybeToken => Login(maybeToken) |> self.send)
            />
          | Some(token) =>
            <JobApp
              submitHandler=(_event => self.send(Logout))
              signOutHandler=(_event => self.send(Logout))
              jwt=token
            />
          } :
          <div />
      )
    </div>,
};
