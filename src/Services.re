type userId = {id: string};

type postingExistsResponse = {status: bool};

type authResponse = {
  iat: int,
  exp: int,
  token: string,
  id: userId,
};

type selfResponse = {
  id: string,
  email: string,
  password: string,
  userType: string,
  verified: bool,
};

type resume = {
  id: string,
  title: string,
  revision: string,
  username: string,
  userId: string,
  downloadUrl: string,
};

module Decode = {
  let companiesArray = json : array(string) =>
    Json.Decode.(json |> array(string));
  let userId = json : userId =>
    Json.Decode.{id: json |> field("_id", string)};
  let authResponse = json : authResponse =>
    Json.Decode.{
      iat: json |> field("iat", int),
      exp: json |> field("exp", int),
      token: json |> field("token", string),
      id: json |> field("user", userId),
    };
  let postingExistsResponse = json : postingExistsResponse =>
    Json.Decode.{status: json |> field("status", bool)};
  let selfResponse = json : selfResponse =>
    Json.Decode.{
      id: json |> field("_id", string),
      email: json |> field("email", string),
      password: json |> field("password", string),
      userType: json |> field("type", string),
      verified: json |> field("verified", bool),
    };
  let resumeRevision = json : resume =>
    Json.Decode.{
      id: json |> field("id", int) |> string_of_int,
      title: json |> field("title", string),
      revision: json |> field("revision", string),
      username: json |> field("user_name", string),
      userId: json |> field("user_id", string),
      downloadUrl: json |> field("download_resume_url", string),
    };
  let resumesArray = json : array(resume) =>
    Json.Decode.(json |> array(resumeRevision));
};

let authenticate = (~email, ~password, ~callback, ~failure, _self) => {
  let authEndpoint = Constants.authUrl ++ "/login";
  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "email", Js.Json.string(email));
  Js.Dict.set(payload, "password", Js.Json.string(password));
  let headers = Fetch.HeadersInit.make({"Content-Type": "application/json"});
  let body =
    Js.Json.object_(payload) |> Js.Json.stringify |> Fetch.BodyInit.make;
  Js.Promise.(
    Fetch.fetchWithInit(
      authEndpoint,
      Fetch.RequestInit.make(~method_=Post, ~body, ~headers, ()),
    )
    |> then_(Fetch.Response.json)
    |> then_(json =>
         json
         |> Decode.authResponse
         |> (
           resp =>
             {
               Js.log(resp);
               callback(
                 Js.Option.some(resp.id.id),
                 Js.Option.some(resp.token),
               );
             }
             |> resolve
         )
       )
    |> catch(err => {
         Js.log(err);
         callback(None, None);
         failure();
         resolve();
       })
  )
  |> ignore;
};

let checkAlreadyApplied = (~url, ~jwt, ~callback, ~failure) => {
  let headers =
    Fetch.HeadersInit.make({
      "Content-Type": "application/json",
      "Authorization": "Bearer " ++ jwt,
    });
  let jobAppURL = Constants.jobAppUrl ++ "/applications/exists?url=" ++ url;
  Js.log("HERE " ++ jwt);
  Js.log("HERE " ++ url);
  Js.Promise.(
    Fetch.fetchWithInit(
      jobAppURL,
      Fetch.RequestInit.make(~method_=Get, ~headers, ~mode=Fetch.CORS, ()),
    )
    |> then_(response => Fetch.Response.json(response))
    |> then_(json =>
         json
         |> Decode.postingExistsResponse
         |> (
           res =>
             res.status ?
               {
                 failure("Already applied to this URL");
                 resolve();
               } :
               {
                 callback();
                 resolve();
               }
         )
       )
    |> catch(err => {
         Js.log(err);
         failure("Connection Error");
         resolve();
       })
  )
  |> ignore;
};

let submit =
    (
      ~company,
      ~position,
      ~url,
      ~resume,
      ~date_posted,
      ~deadline,
      ~status,
      ~id,
      ~jwt,
      ~callback,
      ~failure,
    ) => {
  let jobAppEndpoint = Constants.jobAppUrl ++ "/apply/external";
  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "user_id", Js.Json.string(id));
  Js.Dict.set(payload, "company", Js.Json.string(company));
  Js.Dict.set(payload, "position", Js.Json.string(position));
  Js.Dict.set(payload, "url", Js.Json.string(url));
  Js.Dict.set(payload, "resume", Js.Json.string(resume));
  Js.Dict.set(payload, "date_posted", Js.Json.string(date_posted));
  Js.Dict.set(payload, "deadline", Js.Json.string(deadline));
  Js.Dict.set(payload, "status", Js.Json.string(status));
  let headers =
    Fetch.HeadersInit.make({
      "Content-Type": "application/json",
      "Authorization": "Bearer " ++ jwt,
    });
  Js.log(payload);
  let body =
    Js.Json.object_(payload) |> Js.Json.stringify |> Fetch.BodyInit.make;
  Js.Promise.(
    Fetch.fetchWithInit(
      jobAppEndpoint,
      Fetch.RequestInit.make(
        ~method_=Post,
        ~body,
        ~headers,
        ~mode=Fetch.CORS,
        (),
      ),
    )
    |> then_(response => {
         Js.log(response);
         let status = response |> Fetch.Response.status;
         switch (status) {
         | 200 =>
           callback();
           resolve();
         | _ =>
           failure("Connection Error");
           resolve();
         };
       })
    |> catch(err => {
         Js.log(err);
         failure("Connection Error");
         resolve();
       })
  )
  |> ignore;
};

let submitApplication =
    (
      ~company,
      ~position,
      ~url,
      ~resume,
      ~date_posted,
      ~deadline,
      ~status,
      ~id,
      ~jwt,
      ~callback,
      ~failure,
    ) =>
  /** TODO here before anything check if already applied */
  checkAlreadyApplied(
    ~url,
    ~jwt,
    ~callback=
      _ =>
        submit(
          ~company,
          ~position,
          ~url,
          ~resume,
          ~date_posted,
          ~deadline,
          ~status,
          ~id,
          ~jwt,
          ~callback,
          ~failure,
        ),
    ~failure,
  );

let getResumeRevisions = (~id, ~jwt, ~callback, ~failure) => {
  let headers =
    Fetch.HeadersInit.make({
      "Content-Type": "application/json",
      "Authorization": "Bearer " ++ jwt,
    });
  let resumesEndpoint = Constants.resumesUrl ++ "/resumes/" ++ id;
  Js.log(id);
  Js.log(jwt);
  Js.Promise.(
    Fetch.fetchWithInit(
      resumesEndpoint,
      Fetch.RequestInit.make(~method_=Get, ~headers, ~mode=Fetch.CORS, ()),
    )
    |> then_(response => Fetch.Response.json(response))
    |> then_(json => json |> Decode.resumesArray |> callback |> resolve)
    |> catch(err => {
         Js.log(err);
         failure();
         resolve();
       })
  )
  |> ignore;
};

let validateToken = (~jwt, ~callback) => {
  let selfEndpoint = Constants.authUrl ++ "/users/self";
  let headers =
    Fetch.HeadersInit.make({
      "Content-Type": "application/json",
      "Authorization": "Bearer " ++ jwt,
    });
  Js.Promise.(
    Fetch.fetchWithInit(
      selfEndpoint,
      Fetch.RequestInit.make(~method_=Get, ~headers, ()),
    )
    |> then_(response => {
         Js.log(response);
         let status = response |> Fetch.Response.status;
         switch (status) {
         | 200 =>
           callback(Js.Option.some(jwt));
           resolve();
         | _ =>
           callback(None);
           resolve();
         };
       })
    |> catch(err => {
         Js.log(err);
         callback(None);
         resolve();
       })
  )
  |> ignore;
};

let loadCompanyNames = callback =>
  Js.Promise.(
    Fetch.fetch("business-names.json")
    |> then_(Fetch.Response.json)
    |> then_(json => {
         json |> Decode.companiesArray |> (el => callback(el));
         resolve();
       })
    |> catch(err => {
         Js.log(err);
         Js.Promise.resolve();
       })
  )
  |> ignore;
