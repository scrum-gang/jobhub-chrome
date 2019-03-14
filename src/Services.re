type userId = {id: string};

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
  id: int,
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
      id: json |> field("id", int),
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
             callback(
               Js.Option.some(resp.id.id),
               Js.Option.some(resp.token),
             )
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

let submitApplication =
    (~company, ~position, ~url, ~id, ~jwt, ~callback, ~failure) => {
  let jobAppEndpoint = Constants.jobAppUrl ++ "/apply/external";
  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "company", Js.Json.string(company));
  Js.Dict.set(payload, "position", Js.Json.string(position));
  Js.Dict.set(payload, "url", Js.Json.string(url));
  let headers =
    Fetch.HeadersInit.make({
      "Content-Type": "application/json",
      "Authorization": "Bearer " ++ jwt,
    });
  let body =
    Js.Json.object_(payload) |> Js.Json.stringify |> Fetch.BodyInit.make;
  Js.Promise.(
    Fetch.fetchWithInit(
      jobAppEndpoint,
      Fetch.RequestInit.make(~method_=Post, ~body, ~headers, ()),
    )
    |> then_(response => {
         let status = response |> Fetch.Response.status;
         switch (status) {
         | 200 =>
           callback();
           resolve();
         | _ =>
           failure();
           resolve();
         };
       })
    |> catch(err => {
         Js.log(err);
         failure();
         resolve();
       })
  )
  |> ignore;
};

let getResumeRevisions = (~id, ~jwt, ~callback, ~failure) => {
  let headers =
    Fetch.HeadersInit.make({
      "Content-Type": "application/json",
      "Authorization": "Bearer " ++ jwt,
    });
  let resumesEndpoint = Constants.resumesUrl ++ "/resumes/" ++ id;
  Js.log(id);
  Js.log(jwt);
  Js.log(resumesEndpoint);
  Js.Promise.(
    Fetch.fetchWithInit(
      resumesEndpoint,
      Fetch.RequestInit.make(~method_=Get, ~headers, ()),
    )
    |> then_(Fetch.Response.json)
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
