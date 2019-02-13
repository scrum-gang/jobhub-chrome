module Decode = {
  let companiesArray = json : array(string) =>
    Json.Decode.(json |> array(string));
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
