type authResponse = {
  iat: int,
  exp: int,
  token: string,
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
};

let authenticate = (~email, ~password, ~callback, ~failure, _self) => {
  let authEndpoint = "https://jobhub-authentication-staging.herokuapp.com/login";
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
