open Utilities;

type state = {
  email: string,
  password: string,
  error: bool,
  loading: bool,
};

type action =
  | UpdateEmail(string)
  | UpdatePassword(string)
  | DisplayError
  | Login;

let openRegisterPage = _evt =>
  tabURL(~url=Constants.registerUrl) |> createTab;

let component = ReasonReact.reducerComponent("Login");

let make = (~updateAuth, _children) => {
  ...component,
  reducer: (action, state) =>
    switch (action) {
    | UpdateEmail(email) =>
      ReasonReact.Update({...state, email, error: false})
    | UpdatePassword(pass) =>
      ReasonReact.Update({...state, password: pass, error: false})
    | DisplayError => ReasonReact.Update({...state, error: true})
    | Login =>
      ReasonReact.UpdateWithSideEffects(
        {...state, loading: true, error: false},
        (
          self => {
            let _ =
              Services.authenticate(
                ~email=state.email,
                ~password=state.password,
                ~callback=updateAuth,
                ~failure=() => self.send(DisplayError),
                (),
              );
            ();
          }
        ),
      )
    },
  initialState: () => {email: "", password: "", loading: false, error: false},
  render: self => {
    let errorMessage = self.state.error ? "Invalid credentials" : "";
    <div>
      (
        self.state.loading ?
          <div className="login-size"> <div className="loader" /> </div> :
          <div>
            <form onSubmit=openRegisterPage>
              <button className="btn submit-btn" tabIndex=1>
                ("Register" |> str)
              </button>
            </form>
            <span className="form-vertical-separator">
              <p className="form-vertical-separator-txt"> ("or" |> str) </p>
            </span>
            <p className="error-message"> (errorMessage |> str) </p>
            <form
              onSubmit=(
                ev => {
                  ReactEventRe.Form.preventDefault(ev);
                  self.send(Login);
                }
              )>
              <input
                name="email"
                placeholder="email"
                value=self.state.email
                required=(true |> bool)
                tabIndex=2
                onChange=(ev => UpdateEmail(valueFromEvent(ev)) |> self.send)
              />
              <input
                _type="password"
                name="password"
                placeholder="password"
                value=self.state.password
                required=(true |> bool)
                tabIndex=3
                onChange=(
                  ev => UpdatePassword(valueFromEvent(ev)) |> self.send
                )
              />
              <button className="btn submit-btn" tabIndex=4>
                ("Sign In" |> str)
              </button>
            </form>
          </div>
      )
    </div>;
  },
};
