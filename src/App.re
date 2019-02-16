type state = {token: string};

type action =
  | Login(string)
  | Logout;

let reducer = (action, _state) =>
  switch (action) {
  | Login(jwt) => ReasonReact.Update({token: jwt})
  | Logout => ReasonReact.Update({token: ""})
  };

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  reducer,
  initialState: () => {token: ""},
  render: self =>
    <div className="app">
      (
        (self.state.token === "") ?
          <Login updateToken=(token => self.send(Login(token))) />
          :
          <JobApp
            submitHandler=(_event => self.send(Logout))
            signOutHandler=(_event => self.send(Logout))
          />
      )
    </div>,
};
