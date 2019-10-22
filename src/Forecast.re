open ReactNative;

let styles =
  Style.(
    StyleSheet.create({
      "bigText":
        style(
          ~flex=2.,
          ~fontSize=20.,
          ~textAlign=`center,
          ~margin=10.->dp,
          ~color="#FFFFFF",
          (),
        ),
      "mainText":
        style(
          ~flex=1.,
          ~fontSize=16.,
          ~textAlign=`center,
          ~color="#FFFFFF",
          (),
        ),
    })
  );

[@react.component]
let make = (~main, ~description, ~temp) => {
  <View style=Style.(viewStyle(~flex=1., ()))>
    <Text style=styles##bigText> {main}->React.string </Text>
    <Text style=styles##mainText> {description}->React.string </Text>
    <Text style=styles##bigText>
      {{Js.Float.toString(temp) ++ " C"}->React.string}
    </Text>
  </View>;
};