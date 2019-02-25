open Chrome.Extensions.Storage;

let clear = () => Chrome.Extensions.Storage.Sync.clear(() => ());

/** Store the token if it exists, clear the storage if not
 *  This ensures no expired tokens are kept in storage */
let refreshToken = maybeToken =>
  switch (maybeToken) {
  | Some(jwt) =>
    let token = Js.Dict.empty();
    Js.Dict.set(token, "token", Js.Json.string(jwt));
    Chrome.Extensions.Storage.Sync.set(token, () => ());
  | None => clear()
  };

/** return a promise to retrieve the saved token */
let getSavedToken = () => {
  /** only unwrap a single string attribute */
  let unwrapStoreValue = maybeValue =>
    switch (maybeValue) {
    | Some(Js.Json.JSONString(v)) => Some(v)
    | None => None
    | _ => failwith("Expected token to be a string")
    };
  Js.Promise.make((~resolve, ~reject as _) =>
    Chrome.Extensions.Storage.Sync.get("token", items => {
      let restoredToken =
        /** See https://developer.chrome.com/apps/storage#type-StorageArea
         *  We check chrome.runtime.lastError to ensure correctness even when API call fails */
        Js.Nullable.test(Chrome.Runtime.lastError) ?
          Js.Dict.get(items, "token") |> unwrapStoreValue : None;
      resolve(. restoredToken);
    })
    |> ignore
  );
};
