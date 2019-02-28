import React from 'react';
import {
  AppRegistry,
  StyleSheet,
  View,
  DeviceEventEmitter,
  NativeModules,
} from 'react-native';
import SearchUI from 'browser-core/build/modules/mobile-cards-vertical/SearchUI';
import App from 'browser-core/build/modules/core/app';
import { Provider as CliqzProvider } from 'browser-core/build/modules/mobile-cards/cliqz';

class Cliqz {
  constructor(app) {
    this.app = app;
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

  onAction = ({ module, action, args, id }) => {
    return this.loadingPromise.then(() => {
      return this.state.cliqz.app.modules[module].action(action, ...args).then((response) => {
        if (typeof id !== 'undefined') {
          NativeModules.Bridge.replyToAction(id, response);
        }
        return response;
      });
    });
  }

  componentWillMount() {
    const app = new App();
    this.loadingPromise = app.start().then(async () => {
      await app.ready();
      cliqz = new Cliqz(app);
      this.setState({
        cliqz,
      });
      app.events.sub('search:results', (results) => {
        this.setState({ results })
      })
    });
    DeviceEventEmitter.addListener('action', this.onAction);
  }

  render() {
    const results = this.state.results.results || [];
    const meta = this.state.results.meta || {};
    const theme = this.state.theme;
    return (
      <View style={styles.container}>
        {
          (results.length === 0) || !this.state.cliqz
          ? null
          : (
            <CliqzProvider value={this.state.cliqz}>
              <SearchUI results={results} meta={meta} theme="light" />
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
  }
});

AppRegistry.registerComponent('BrowserCoreApp', () => BrowserCoreApp);
