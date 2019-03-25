open Utilities;

type resume = Services.resume;

type state = {
  url: string,
  company: string,
  position: string,
  postedDate: string,
  deadline: string,
  companies: array(string),
  resumes: array(resume),
  resumeValue: string,
  status: string,
  id: string,
  jwt: string,
  error: bool,
  success: bool,
  loading: bool,
};

type action =
  | UpdateUrl(string)
  | UpdateCompany(string)
  | UpdatePosition(string)
  | UpdatePostedDate(string)
  | UpdateDeadline(string)
  | UpdateCompanyNames(array(string))
  | UpdateResumes(array(resume))
  | UpdateResumeValue(string)
  | UpdateStatus(string)
  | Submit
  | SuccesfulSubmit
  | FailedSubmit;

let reducer = (action, state) =>
  switch (action) {
  | UpdateUrl(value) =>
    ReasonReact.Update({...state, url: value, error: false})
  | UpdateCompany(value) =>
    ReasonReact.Update({...state, company: value, error: false})
  | UpdatePosition(value) =>
    ReasonReact.Update({...state, position: value, error: false})
  | UpdatePostedDate(value) =>
    ReasonReact.Update({...state, postedDate: value, error: false})
  | UpdateDeadline(value) =>
    ReasonReact.Update({...state, deadline: value, error: false})
  | UpdateCompanyNames(value) =>
    ReasonReact.Update({...state, companies: value})
  | UpdateResumes(value) => ReasonReact.Update({...state, resumes: value})
  | UpdateResumeValue(value) =>
    ReasonReact.Update({...state, resumeValue: value, error: false})
  | UpdateStatus(value) =>
    ReasonReact.Update({...state, status: value, error: false})
  | Submit =>
    ReasonReact.UpdateWithSideEffects(
      {...state, loading: true, error: false},
      (
        self =>
          Services.submitApplication(
            ~company=state.company,
            ~position=state.position,
            ~url=state.url,
            ~resume=state.resumeValue,
            ~date_posted=state.postedDate,
            ~deadline=state.deadline,
            ~status=state.status,
            ~id=state.id,
            ~jwt=state.jwt,
            ~callback=() => self.send(SuccesfulSubmit),
            ~failure=() => self.send(FailedSubmit),
          )
      ),
    )
  | SuccesfulSubmit =>
    ReasonReact.Update({
      ...state,
      error: false,
      success: true,
      loading: false,
    })
  | FailedSubmit =>
    ReasonReact.Update({
      ...state,
      error: true,
      success: false,
      loading: false,
    })
  };

let component = ReasonReact.reducerComponent("JobApp");

let make = (~signOutHandler, ~id, ~jwt, _children) => {
  ...component,
  reducer,
  initialState: () => {
    url: "",
    company: "",
    position: "",
    postedDate: "",
    deadline: "",
    companies: [||],
    resumes: [||],
    resumeValue: "",
    status: "",
    id,
    jwt,
    error: false,
    success: false,
    loading: false,
  },
  didMount: self => {
    let setCompanyNames = x => UpdateCompanyNames(x) |> self.send;
    let setResumes = x => UpdateResumes(x) |> self.send;
    ReasonReact.SideEffects(
      _self => {
        /** Fetch all data required for the form */
        Services.loadCompanyNames(setCompanyNames);
        Services.getResumeRevisions(
          ~id, ~jwt, ~callback=setResumes, ~failure=_ =>
          Js.log("Failed to load resumes")
        );
      },
    );
  },
  render: self => {
    /** Event handlers which function as sort of dispatchers */
    let changeUrl = x => UpdateUrl(x) |> self.send;
    let changeCompany = x => UpdateCompany(x) |> self.send;
    let changePosition = x => UpdatePosition(x) |> self.send;
    let changePostedDate = x => UpdatePostedDate(x) |> self.send;
    let changeDeadline = x => UpdateDeadline(x) |> self.send;
    let changeResumeValue = x => UpdateResumeValue(x) |> self.send;
    let changeStatusValue = x => UpdateStatus(x) |> self.send;
    let errorMessage = self.state.error ? "Failed to add application" : "";
    let successMessage = self.state.success ? "Added application" : "";
    self.state.loading ?
      <div className="jobapp-size"> <div className="loader" /> </div> :
      <div>
        <form
          onSubmit=(
            ev => {
              ReactEventRe.Form.preventDefault(ev);
              self.send(Submit);
            }
          )>
          <p className="error-message"> (errorMessage |> str) </p>
          <p className="success-message"> (successMessage |> str) </p>
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
                required=(Js.Boolean.to_js_boolean(true))
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
            required=(Js.Boolean.to_js_boolean(true))
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
            required=(Js.Boolean.to_js_boolean(true))
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
              required=(Js.Boolean.to_js_boolean(true))
            />
          </div>
          <div className="form-horizontal-separator">
            <label> ("Deadline" |> str) </label>
            <input
              _type="date"
              name="deadline"
              value=self.state.deadline
              required=(Js.Boolean.to_js_boolean(true))
              onChange=(ev => ev |> Utilities.valueFromEvent |> changeDeadline)
            />
          </div>
          <div className="form-horizontal-separator">
            <label>
              <input
                _type="radio"
                name="status"
                value="APPLIED"
                required=(Js.Boolean.to_js_boolean(true))
                onChange=(ev => valueFromEvent(ev) |> changeStatusValue)
              />
              ("Applied" |> str)
            </label>
            <label>
              <input
                _type="radio"
                name="status"
                value="TO_APPLY"
                onChange=(ev => valueFromEvent(ev) |> changeStatusValue)
              />
              ("To apply" |> str)
            </label>
          </div>
          <select
            id="resumes"
            value=self.state.resumeValue
            required=(Js.Boolean.to_js_boolean(true))
            onChange=(
              evt => Utilities.valueFromEvent(evt) |> changeResumeValue
            )>
            <option key="" value=""> ("CV used" |> str) </option>
            (
              ReasonReact.arrayToElement(
                self.state.resumes
                |> Array.map((el: resume) =>
                     <option key=el.id value=el.id>
                       (el.title ++ " " ++ el.revision |> str)
                     </option>
                   ),
              )
            )
          </select>
          <button className="btn submit-btn"> ("Submit" |> str) </button>
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
