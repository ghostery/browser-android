import React from 'react';
import {AppRegistry, StyleSheet, Text, View, DeviceEventEmitter, } from 'react-native';
import SearchUI from 'browser-core/build/modules/mobile-cards/SearchUI';
import { Provider as CliqzProvider } from 'browser-core/build/modules/mobile-cards/cliqz';
import Cliqz from './cliqz';

class BrowserCoreApp extends React.Component {
  constructor(props) {
    super(props);
    this.state = { results: [] };
    this.cliqz = new Cliqz();
  }

  componentWillMount() {
    DeviceEventEmitter.addListener(
      'search:renderResults',
      (results) => this.setState({ results })
    );
  }

  render() {
    return (
      <View style={styles.container}>
        {this.state.results.length === 0
          ? <Text>No results</Text>
          : (
            <CliqzProvider value={this.cliqz}>
              <SearchUI results={this.state.results} theme="dark" />
            </CliqzProvider>
          )
        }
      </View>
    );
  }
}
var styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    backgroundColor: 'white',
  },
  hello: {
    fontSize: 20,
    textAlign: 'center',
    margin: 10,
  },
});

AppRegistry.registerComponent('BrowserCoreApp', () => BrowserCoreApp);
