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
      password: json |> field("token", string),
      userType: json |> field("type", string),
      verified: json |> field("verified", bool),
    };
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

let validateToken = (~jwt, ~callback, ~failure) => {
  let selfEndpoint = Constants.authUrl ++ "/self";
  let headers =
    Fetch.HeadersInit.make({
      "Content-Type": "application/json",
      "Authorization": "Bearer " ++ jwt,
    });
  let body =
    Js.Json.object_(Js.Dict.empty())
    |> Js.Json.stringify
    |> Fetch.BodyInit.make;
  Js.Promise.(
    Fetch.fetchWithInit(
      selfEndpoint,
      Fetch.RequestInit.make(~method_=Get, ~body, ~headers, ()),
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
