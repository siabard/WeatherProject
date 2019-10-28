open ReactNative;

type weather_main = {temp: float};

type weather_detail = {
  id: int,
  main: string,
  description: string,
};

type weather_format = {
  main: option(weather_main),
  weather: option(array(weather_detail)),
};

module Decode = {
  let weather_main = json =>
    Json.Decode.{temp: json |> field("temp", Json.Decode.float)};

  let weather_detail = json =>
    Json.Decode.{
      id: json |> field("id", int),
      main: json |> field("main", string),
      description: json |> field("description", string),
    };

  let weather_format = json =>
    Json.Decode.{
      main: json |> optional(field("main", weather_main)),
      weather: json |> optional(field("weather", array(weather_detail))),
    };
};
let api_key = "bbeb34ebf60ad50f7893e7440a1e2b0b";

let base_font_size = 16.->Style.dp;
let styles =
  Style.(
    StyleSheet.create({
      "container":
        style(
          ~flex=1.,
          ~justifyContent=`center,
          ~alignItems=`center,
          ~backgroundColor="#4D4D4D",
          (),
        ),
      "backdrop": style(~flex=1., ~flexDirection=`column, ()),
      "overlay":
        style(
          ~paddingTop=5.->dp,
          ~backgroundColor="#000000",
          ~opacity=0.5,
          ~flexDirection=`column,
          ~alignItems=`center,
          (),
        ),
      "row":
        style(
          ~flex=1.,
          ~flexDirection=`row,
          ~flexWrap=`nowrap,
          ~alignItems=`flexStart,
          ~padding=30.->dp,
          (),
        ),
      "zipContainer":
        style(
          ~borderBottomColor="#DDDDDD",
          ~borderBottomWidth=1.,
          ~marginLeft=5.->dp,
          ~marginTop=3.->dp,
          (),
        ),
      "zipCode":
        style(~width=50.->dp, ~height=base_font_size, ~padding=0.->dp, ()),
      "welcome":
        style(
          ~flex=1.,
          ~fontSize=20.,
          ~textAlign=`center,
          ~margin=10.->dp,
          (),
        ),
      "mainText": style(~fontSize=16., ~color="#FFFFFF", ()),
      "input":
        style(~flex=1., ~fontSize=20., ~borderWidth=2., ~height=40.->dp, ()),
      "location": style(~flex=1., ~height=40.->dp, ~width=40.->dp, ()),
    })
  );

type forecast_type = {
  main: string,
  description: string,
  temp: float,
};

[@react.component]
let make = () => {
  let (zipcode, setZipcode) = React.useState(() => "");
  let (forecast: forecast_type, setForecast) =
    React.useState(() => {main: "", description: "", temp: 0.});

  let submitEdit: ReactNative.TextInput.editingEvent => unit =
    event => {
      let message = event##nativeEvent##text;

      Axios.get(
        "http://api.openweathermap.org/data/2.5/weather?q="
        ++ message
        ++ "&units=imperial&APPID="
        ++ api_key,
      )
      |> Js.Promise.then_(res =>
           Js.Promise.resolve(Decode.weather_format(res##data))
         )
      |> Js.Promise.then_(value =>
           Js.Promise.resolve(
             setForecast(_ =>
               {
                 main:
                   switch (value.weather) {
                   | None => ""
                   | Some(weather_details) => weather_details[0].main
                   },
                 description:
                   switch (value.weather) {
                   | None => ""
                   | Some(weather_details) => weather_details[0].description
                   },
                 temp:
                   switch (value.main) {
                   | None => 0.
                   | Some(main) => (main.temp -. 32.0) *. 5.0 /. 9.0
                   },
               }
             ),
           )
         )
      |> Js.Promise.catch(_err =>
           Js.Promise.resolve(
             setForecast(_ => {main: "", description: "Not found", temp: 0.}),
           )
         )
      |> ignore;

      setZipcode(_ => message);
      // setForecast(_ => {main: "1", description: "test", temp: 10.});
      ();
    };

  let getCoords = (lat, lon) =>
    if (lat > (-1.0) && lon > (-1.0)) {
      Axios.get(
        "http://api.openweathermap.org/data/2.5/weather?lat="
        ++ Js.Float.toString(lat)
        ++ "&lon="
        ++ Js.Float.toString(lon)
        ++ "&units=imperial&APPID="
        ++ api_key,
      )
      |> Js.Promise.then_(res =>
           Js.Promise.resolve(Decode.weather_format(res##data))
         )
      |> Js.Promise.then_(value =>
           Js.Promise.resolve(
             setForecast(_ =>
               {
                 main:
                   switch (value.weather) {
                   | None => ""
                   | Some(weather_details) => weather_details[0].main
                   },
                 description:
                   switch (value.weather) {
                   | None => ""
                   | Some(weather_details) => weather_details[0].description
                   },
                 temp:
                   switch (value.main) {
                   | None => 0.
                   | Some(main) => (main.temp -. 32.0) *. 5.0 /. 9.0
                   },
               }
             ),
           )
         )
      |> Js.Promise.catch(_err =>
           Js.Promise.resolve(
             setForecast(_ => {main: "", description: "Not found", temp: 0.}),
           )
         )
      |> ignore;
      ();
    };

  <View style=styles##container>
    <ImageBackground
      source={Image.Source.fromRequired(
        Packager.require("./image/astronomy.jpg"),
      )}
      resizeMode=`cover
      style=styles##backdrop>
      <View style=styles##overlay>
        <View style=styles##row>
          <View style=styles##zipContainer>
            <TextInput
              style=Style.(array([|styles##input, styles##mainText|]))
              returnKeyType=`go
              placeholder="Register"
              onSubmitEditing=submitEdit
            />
            <View style=styles##location>
              <LocationButton onGetCoords=getCoords />
            </View>
          </View>
        </View>
        {switch (forecast.main) {
         | "" => <View />
         | _ =>
           <Forecast
             main={forecast.main}
             description={forecast.description}
             temp={forecast.temp}
           />
         }}
      </View>
    </ImageBackground>
  </View>;
};