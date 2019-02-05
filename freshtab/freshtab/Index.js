import React from 'react';
import { View, StyleSheet, Text } from 'react-native';
import AppNavigator from './navigation/AppNavigator';

export default class Index extends React.Component {
  render() {
    return (
      <View style={styles.container}>
        <AppNavigator />
      </View>
    )
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#000'
  }
});
