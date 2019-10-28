open ReactNative;

type ords;
[@bs.new] [@bs.module "./LocationWrapper.js"]
external wrapper: unit => ords = "default";

[@bs.send] external get_lon: ords => float = "get_lon";
[@bs.send] external get_lat: ords => float = "get_lat";
[@bs.send] external get_coords: ords => unit = "get_coords";
let wrapperInstance = wrapper();

[@react.component]
let make = (~onGetCoords) => {
  //Js.log("LocationButton");

  let onPress = _event => {
    get_coords(wrapperInstance);

    let lat = get_lat(wrapperInstance);
    let lon = get_lon(wrapperInstance);

    onGetCoords(lat, lon);
    ();
  };

  <Button title={js|위치|js} onPress />;
};