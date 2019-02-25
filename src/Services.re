type authResponse = {
  iat: int,
  exp: int,
  token: string,
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
  let authResponse = json : authResponse =>
    Json.Decode.{
      iat: json |> field("iat", int),
      exp: json |> field("exp", int),
      token: json |> field("token", string),
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
         |> (resp => callback(Js.Option.some(resp.token)) |> resolve)
       )
    |> catch(err => {
         Js.log(err);
         callback(None);
         failure();
         resolve();
       })
  )
  |> ignore;
};

let getResumeRevisions = (~jwt, ~callback, ~failure) => {
  let headers =
    Fetch.HeadersInit.make({
      "Content-Type": "application/json",
      "Authorization": "Bearer " ++ jwt,
    });
  let getResumeRevisionPromise = (user: selfResponse) => {
    let resumesEndpoint = Constants.resumesUrl ++ "/resumes/" ++ user.id;
    Js.Promise.(
      Fetch.fetchWithInit(
        resumesEndpoint,
        Fetch.RequestInit.make(~method_=Get, ~headers, ()),
      )
      |> then_(Fetch.Response.json)
      |> then_(json => json |> Decode.resumesArray |> callback |> resolve)
    );
  };
  let selfEndpoint = Constants.authUrl ++ "/users/self";
  Js.Promise.(
    Fetch.fetchWithInit(
      selfEndpoint,
      Fetch.RequestInit.make(~method_=Get, ~headers, ()),
    )
    |> then_(Fetch.Response.json)
    |> then_(json => json |> Decode.selfResponse |> resolve)
    |> then_(getResumeRevisionPromise)
    |> catch(err => {
         Js.log(err);
         failure();
         resolve();
       })
  )
  |> ignore;
};

let validateToken = (~jwt, ~callback, ~failure) => {
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
           resolve(true);
         | _ =>
           failure(jwt);
           resolve(false);
         };
       })
    |> catch(err => {
         Js.log(err);
         callback(None);
         failure(jwt);
         resolve(false);
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
    |> catch(_err => {
         Js.log(_err);
         Js.Promise.resolve();
       })
  )
  |> ignore;
