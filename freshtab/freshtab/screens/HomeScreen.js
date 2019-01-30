import React from 'react';
import { View, Text, StyleSheet, FlatList, NativeModules } from 'react-native';
import Icon from 'browser-core/build/modules/mobile-cards/components/partials/Icon';

const DOMAINS = [
  {
    title: 'Youtube',
    url: 'https://www.youtube.com',
    lastVisited: 'today',
    logoDetails: {
      backgroundColor: '800000',
      text: 'YT'
    }
  },
  {
    title: 'Wikipedia',
    url: 'https://www.wikipedia.com',
    lastVisited: 'today at 11:00',
    logoDetails: {
      backgroundColor: '999999',
      text: 'WI'
    }
  },
  {
    title: 'Facebook',
    url: 'https://www.wikipedia.com',
    lastVisited: 'yesterday at 12:00',
    logoDetails: {
      backgroundColor: '5048A5',
      text: 'FB'
    }
  },
  {
    title: '9gag',
    url: 'https://www.9gag.com',
    lastVisited: '3 hours ago',
    logoDetails: {
      backgroundColor: '464649',
      text: '9g'
    }
  },
  {
    title: 'Wikipedia',
    url: 'https://www.wikipedia.com',
    lastVisited: 'today at 11:00',
    logoDetails: {
      backgroundColor: '179471',
      text: 'WI'
    }
  },
  {
    title: 'Facebook',
    url: 'https://www.wikipedia.com',
    lastVisited: 'yesterday at 12:00',
    logoDetails: {
      backgroundColor: 'F2800C',
      text: 'FB'
    }
  },
  {
    title: '9gag',
    url: 'https://www.9gag.com',
    lastVisited: '3 hours ago',
    logoDetails: {
      backgroundColor: '464649',
      text: '9g'
    }
  },
  {
    title: 'Wikipedia',
    url: 'https://www.wikipedia.com',
    lastVisited: 'today at 11:00',
    logoDetails: {
      backgroundColor: '999999',
      text: 'WI'
    }
  },
  {
    title: 'Facebook',
    url: 'https://www.wikipedia.com',
    lastVisited: 'yesterday at 12:00',
    logoDetails: {
      backgroundColor: '5048A5',
      text: 'FB'
    }
  }
]

export default class HomeScreen extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      historyDials: []
    };
  }

  async componentDidMount() {
    // TODO
    // 1. return empty array from native code
    // 2. return an array of speed dials with below structure
    const historyDials = await NativeModules.HistoryDials.getTopSites();
    this.setState({
      historyDials
    });
    console.log(historyDials, '!!historyDials');
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

    return (
      <View style={itemStyles.container}>
        {/*<Icon
          width={60}
          height={60}
          logoDetails={data.logoDetails}
        />*/}
        <View style={itemStyles.rightContainer}>
          <Text style={itemStyles.title}>{data.title}</Text>
          <Text>{data.lastVisited}</Text>
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
