type state = {token: option(string)};

type action =
  | Login(string)
  | Logout;

let reducer = (action, _state) =>
  switch (action) {
  | Login(jwt) => ReasonReact.Update({token: Js.Option.some(jwt)})
  | Logout => ReasonReact.Update({token: None})
  };

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  reducer,
  initialState: () => {token: None},
  render: self =>
    <div className="app">
      (
        switch (self.state.token) {
        | None =>
          <Login updateToken=(
            token =>
              switch (token) {
              | Some(token) => self.send(Login(token))
              | None => self.send(Logout)
              }
            ) />
        | Some(_token) =>
          <JobApp
            submitHandler=(_event => self.send(Logout))
            signOutHandler=(_event => self.send(Logout))
          />
        }
      )
    </div>,
};
