type route =
  | LoginPage
  | JobAppPage;

type state = {route};

type action =
  | Login
  | Logout;

let reducer = (action, _state) =>
  switch (action) {
  | Login => ReasonReact.Update({route: JobAppPage})
  | Logout => ReasonReact.Update({route: LoginPage})
  };

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  reducer,
  initialState: () => {route: LoginPage},
  render: self =>
    <div className="app">
      (
        switch (self.state.route) {
        | LoginPage => <Login submitHandler=(_event => self.send(Login)) />
        | JobAppPage => <JobApp
                          submitHandler=(_event => self.send(Logout))
                          signOutHandler=(_event => self.send(Logout)) />
        }
      )
    </div>,
};
