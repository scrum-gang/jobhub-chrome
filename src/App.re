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
    <div>
      (
        switch (self.state.route) {
        | Login => <Login />
        | JobApp => <JobApp />
        }
      )
      <div>
        <button
          onClick=(
            switch (self.state.route) {
            | Login => (_event => self.send(Login))
            | JobApp => (_event => self.send(Logout))
            }
          )>
          (
            ReasonReact.stringToElement(
              switch (self.state.route) {
              | Login => "Login"
              | JobApp => "Logout"
              },
            )
          )
        </button>
      </div>
    </div>,
};
