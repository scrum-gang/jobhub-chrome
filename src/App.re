type route =
  | Login
  | JobApp;

type state = {route};

type action =
  | Login
  | Logout;

let reducer = (action, _state) =>
  switch (action) {
  | Login => ReasonReact.Update({route: JobApp})
  | Logout => ReasonReact.Update({route: Login})
  };

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  reducer,
  initialState: () => {route: Login},
  render: self =>
    <div className="app">
      (
        switch (self.state.route) {
        | Login => <Login submitHandler=(_event => self.send(Login)) />
        | JobApp => <JobApp submitHandler=(_event => self.send(Logout))
                            signOutHandler=(_event => self.send(Logout)) />
        }
      )
    </div>,
};
