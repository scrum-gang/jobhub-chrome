open Utilities;

type state = {
  email: string,
  password: string,
  error: bool,
};

type action =
  | UpdateEmail(string)
  | UpdatePassword(string)
  | DisplayError
  | Login;

/** TODO: a binding is better because it offers type-safety */
let openSignupPage = [%raw
  {|
  function(url) {
    chrome.tabs.create({ url: "http://stackoverflow.com/" });
  }
|}
];

let component = ReasonReact.reducerComponent("Login");

let make = (~updateToken, _children) => {
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
        {...state, error: false},
        (
          self => {
            let _ =
              Services.authenticate(
                ~email=state.email,
                ~password=state.password,
                ~callback=updateToken,
                ~failure=() => self.send(DisplayError),
                (),
              );
            ();
          }
        ),
      )
    },
  initialState: () => {email: "", password: "", error: false},
  render: self => {
    let errorMessage = self.state.error ? "Invalid credentials" : "";
    <div>
      <form onSubmit=openSignupPage>
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
          onChange=(ev => UpdateEmail(valueFromEvent(ev)) |> self.send)
          tabIndex=2
        />
        <input
          _type="password"
          name="password"
          placeholder="password"
          value=self.state.password
          onChange=(ev => UpdatePassword(valueFromEvent(ev)) |> self.send)
          tabIndex=3
        />
        <button className="btn submit-btn" tabIndex=4>
          ("Sign In" |> str)
        </button>
      </form>
    </div>;
  },
};
