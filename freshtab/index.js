import React from 'react';
import {AppRegistry, StyleSheet, Text, View, DeviceEventEmitter, } from 'react-native';
import SearchUI from 'browser-core/build/modules/mobile-cards/SearchUI';
import App from 'browser-core/build/modules/core/app';
import { Provider as CliqzProvider } from 'browser-core/build/modules/mobile-cards/cliqz';
import Freshtab from './freshtab/Index';

class Cliqz {
  constructor(app) {
    this.mobileCards = app.modules['mobile-cards'].background.actions;
    this.geolocation = app.modules['geolocation'].background.actions;
    this.search = app.modules['search'].background.actions;
    this.core = app.modules['core'].background.actions;
  }
}

class BrowserCoreApp extends React.Component {
  constructor(props) {
    super(props);
    this.state = { results: [], cliqz: null };
  }

  componentWillMount() {
    const app = new App();
    let cliqz;
    const start = Date.now();
    const loadingPromise = app.start().then(async () => {
      await app.ready();
      cliqz = new Cliqz(app);
      this.setState({
        cliqz,
      });
      app.events.sub('search:results', (results) => {
        this.setState({ results: results.results })
      })
    });

    DeviceEventEmitter.addListener(
      'search:search',
      (query) => loadingPromise.then(() => cliqz.search.startSearch(query)),
    );
  }

  render() {
    const results = this.state.results || [];
    const theme = this.state.theme;
    return (
      <View style={styles.container}>
        {
          (results.length === 0) || !this.state.cliqz
          ? null
          : (
            <CliqzProvider value={this.state.cliqz}>
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
