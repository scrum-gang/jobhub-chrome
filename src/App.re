type route =
  | JobAppLogin
  | JobApp;

type state = {route};

type action =
  | Login
  | Logout;

let reducer = (action, _state) =>
  switch (action) {
  | Login => ReasonReact.Update({route: JobApp})
  | Logout => ReasonReact.Update({route: JobAppLogin})
  };

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  reducer,
  initialState: () => {route: JobAppLogin},
  render: self =>
    <div className="app">
      (
        switch (self.state.route) {
        | JobAppLogin => <Login submitHandler=(_event => self.send(Login)) />
        | JobApp => <JobApp
                          submitHandler=(_event => self.send(Logout))
                          signOutHandler=(_event => self.send(Logout)) />
        }
      )
    </div>,
};
