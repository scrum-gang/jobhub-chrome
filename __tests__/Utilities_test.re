open Jest;

open Expect;

open Utilities;

describe("Utility Functions", () => {
  describe("str", () => {
    test("with empty string", () => {
      let text = "";
      let textElement = ReasonReact.stringToElement(text);
      str(text) |> expect |> toEqual(textElement);
    });
    test("with non-empty string", () => {
      let text = "abc132!~";
      let textElement = ReasonReact.stringToElement(text);
      str(text) |> expect |> toEqual(textElement);
    });
  });
  describe("bool", () => {
    test("with true", () => {
      let jsBool = Js.Boolean.to_js_boolean(true);
      bool(true) |> expect |> toEqual(jsBool);
    });
    test("with false", () => {
      let jsBool = Js.Boolean.to_js_boolean(false);
      bool(false) |> expect |> toEqual(jsBool);
    });
  });
});
