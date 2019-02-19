open Utilities;

type state = {
  url: string,
  company: string,
  position: string,
  postedDate: string,
  companies: array(string),
};

type action =
  | UpdateUrl(string)
  | UpdateCompany(string)
  | UpdatePosition(string)
  | UpdatePostedDate(string)
  | UpdateCompanyNames(array(string));

let reducer = (action, _state) =>
  switch (action) {
  | UpdateUrl(value) => ReasonReact.Update({..._state, url: value})
  | UpdateCompany(value) => ReasonReact.Update({..._state, company: value})
  | UpdatePosition(value) => ReasonReact.Update({..._state, position: value})
  | UpdatePostedDate(value) =>
    ReasonReact.Update({..._state, postedDate: value})
  | UpdateCompanyNames(value) =>
    ReasonReact.Update({..._state, companies: value})
  };

let component = ReasonReact.reducerComponent("JobApp");

let make = (~submitHandler, ~signOutHandler, _children) => {
  ...component, /* spread the template's other defaults into here  */
  reducer,
  initialState: () => {
    url: "",
    company: "",
    position: "",
    postedDate: "",
    companies: [||],
  },
  didMount: self => {
    let setCompanyNames = x => self.send(UpdateCompanyNames(x));
    ReasonReact.UpdateWithSideEffects(
      self.state,
      _self => Services.loadCompanyNames(setCompanyNames),
    );
  },
  render: self => {
    /** Event handlers which function as sort of dispatchers */
    let changeUrl = x => self.send(UpdateUrl(x));
    let changeCompany = x => self.send(UpdateCompany(x));
    let changePosition = x => self.send(UpdatePosition(x));
    let changePostedDate = x => self.send(UpdatePostedDate(x));
    <div>
      <form>
        (
          switch (self.state.companies) {
          | [||] => "Loading" |> str
          | _ =>
            <ScrapingInput
              script=ScrapingFunctions.scriptHtmlBody
              typeValue="text"
              validationFn=ScrapingFunctions.validateNonNull
              processFn=(
                x =>
                  ScrapingFunctions.extractCompaniesProcess(
                    self.state.companies,
                    x,
                  )
              )
              reducerFn=changeCompany
              name="company"
              placeholder="company"
              value=self.state.company
            />
          }
        )
        <ScrapingInput
          script=ScrapingFunctions.scriptPosition
          typeValue="text"
          validationFn=ScrapingFunctions.validateNonNull
          processFn=ScrapingFunctions.toStringProcess
          reducerFn=changePosition
          name="position"
          placeholder="position"
          value=self.state.position
        />
        <ScrapingInput
          script=ScrapingFunctions.scriptUrl
          typeValue="url"
          validationFn=ScrapingFunctions.validateUrl
          processFn=ScrapingFunctions.toStringProcess
          reducerFn=changeUrl
          name="url"
          placeholder="url"
          value=self.state.url
        />
        <div className="form-horizontal-separator">
          <label> ("Date posted" |> str) </label>
          <ScrapingInput
            script=ScrapingFunctions.scriptHtmlBody
            typeValue="date"
            validationFn=ScrapingFunctions.validateDate
            processFn=ScrapingFunctions.extractPostedDateProcess
            reducerFn=changePostedDate
            name="postedDate"
            placeholder=""
            value=self.state.postedDate
          />
        </div>
        <div className="form-horizontal-separator">
          <label> ("Deadline" |> str) </label>
          <input _type="date" name="deadline" />
        </div>
        <div className="form-horizontal-separator">
          <label>
            <input _type="radio" name="status" value="applied" />
            ("Applied" |> str)
          </label>
          <label>
            <input _type="radio" name="status" value="toApply" />
            ("To apply" |> str)
          </label>
        </div>
        <select>
          <option value="0"> ("Used CV" |> str) </option>
          <option value="1"> ("TODO" |> str) </option>
          <option value="2"> ("TODO" |> str) </option>
        </select>
        <button className="btn submit-btn" onClick=submitHandler>
          ("Submit" |> str)
        </button>
        <span className="form-vertical-separator">
          <p className="form-vertical-separator-txt"> ("or" |> str) </p>
        </span>
        <button className="btn signout-btn" onClick=signOutHandler>
          ("Sign Out" |> str)
        </button>
      </form>
    </div>;
  },
};
