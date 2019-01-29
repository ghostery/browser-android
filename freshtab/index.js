import React from 'react';
import {AppRegistry, StyleSheet, Text, View, DeviceEventEmitter, } from 'react-native';
import SearchUI from 'browser-core/build/modules/mobile-cards/SearchUI';
import { Provider as CliqzProvider } from 'browser-core/build/modules/mobile-cards/cliqz';
import Cliqz from './cliqz';

class BrowserCoreApp extends React.Component {
  constructor(props) {
    super(props);
    this.state = { results: { results: '[]' } };
    this.cliqz = new Cliqz();
  }

  componentWillMount() {
    DeviceEventEmitter.addListener(
      'search:renderResults',
      (results) => this.setState({ results })
    );
  }

  render() {
    const ts5 = Date.now();
    const results = JSON.parse(this.state.results.results || '[]') ;
    const ts1 = this.state.results.ts1;
    const ts2 = this.state.results.ts2;
    const ts3 = this.state.results.ts3;
    const ts4 = this.state.results.ts4;
    const ts6 = Date.now();
    const theme = this.state.theme;
    return (
      <View style={styles.container}>
        <Text>1: {ts1}</Text>
        <Text>2: {ts2}</Text>
        <Text>3: {ts3}</Text>
        <Text>4: {ts4}</Text>
        <Text>5: {ts5}</Text>
        <Text>6: {ts6}</Text>
        {results.length === 0
          ? <Text>No results</Text>
          : (
            <CliqzProvider value={this.cliqz}>
              <SearchUI results={results} theme="dark" />
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
  },
  hello: {
    fontSize: 20,
    textAlign: 'center',
    margin: 10,
  },
});

AppRegistry.registerComponent('BrowserCoreApp', () => BrowserCoreApp);
