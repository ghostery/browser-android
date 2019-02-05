import React from 'react';
import { View, Text, StyleSheet } from 'react-native';

export default class Bikini extends React.Component {
  render() {
    return (
      <View style={ styles.container }>
        <Text>Third Screen</Text>
      </View>
    )
  }
}

var styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
  }
});
