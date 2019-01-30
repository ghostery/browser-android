import React from 'react';
import { View, Text, StyleSheet, FlatList, NativeModules } from 'react-native';
import Icon from 'browser-core/build/modules/mobile-cards/components/partials/Icon';
import LogoDB from 'browser-core/build/modules/core/logo-database.json';
import moment from 'moment';
import getLogo from 'cliqz-logo-database';

const LOGO_DB_VERSION = 1521469421408;

function calculateDisplayDate(timestamp) {
  return moment(parseInt(timestamp)).fromNow();
}

export default class HomeScreen extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      historyDials: []
    };
  }

  async componentDidMount() {
    const historyDials = await NativeModules.HistoryDials.getTopSites();
    this.setState({
      historyDials
    });
  }

  render() {
    return (
      <FlatList
        style={styles.list}
        data={this.state.historyDials}
        renderItem={this.renderItem}
      />
    )
  }

  renderItem = ({ item }) => (
    <HistoryItem data={item} />
  )
}

class HistoryItem extends React.Component {
  render() {
    const data = this.props.data;
    const logo = getLogo(data.url, {
      database: LogoDB,
      version: LOGO_DB_VERSION,
    }) || {};
    const logoDetails = {
      backgroundColor: logo.color || '000000',
      backgroundImage: 'url('+logo.logoUrl+')',
      text: data.title.slice(0, 2) || ''
    }

    return (
      <View style={itemStyles.container}>
        <Icon
          width={60}
          height={60}
          logoDetails={logoDetails}
        />
        <View style={itemStyles.rightContainer}>
          <Text style={itemStyles.title}>{data.title}</Text>
          <Text>{calculateDisplayDate(data.lastVisited)}</Text>
        </View>
      </View>
    )
  }
}

var styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
  },
  list: {
    paddingTop: 10
  }
});

var itemStyles = StyleSheet.create({
  container: {
    flexDirection: 'row',
    marginTop: 10,
    marginLeft: 30,
    borderBottomWidth: 1,
    borderBottomColor: '#666666',
    paddingBottom: 9
  },
  title: {
    color: '#ffffff',
    fontSize: 20
  },
  rightContainer: {
    marginLeft: 10,
    flex: 1,
    justifyContent: 'center'
  }
})
