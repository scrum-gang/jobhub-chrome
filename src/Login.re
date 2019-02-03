let component = ReasonReact.statelessComponent("Login");

let make = (~submitHandler, _children) => {
  ...component,
  render: _self => <div>
    <form>
      <button className="btn submit-btn" onClick=submitHandler tabIndex=1>
        (ReasonReact.stringToElement("Sign In with Google"))
      </button>
    </form>

    <span className="form-separator">
      <p className="form-separator-txt">
        (ReasonReact.stringToElement("or"))
      </p>
    </span>

    <form>
      <input name="email" placeholder="email" tabIndex=2 />
      /* TODO: can't add type_="password". not sure why not. */
      <input name="password" placeholder="password" tabIndex=3 />
      <button className="btn submit-btn" onClick=submitHandler tabIndex=4>
        (ReasonReact.stringToElement("Sign In"))
      </button>
    </form>
  </div>,
};
