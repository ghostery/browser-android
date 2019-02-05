import React from 'react';
import { View, Text, StyleSheet, FlatList } from 'react-native';
import Icon from 'browser-core/build/modules/mobile-cards/components/partials/Icon';

const NEWS = [
  {
    description: 'The US and China have reached a 10-point trade deal that opens the Chinese market to US',
    url: 'reuters.com',
    lastVisited: '1 hour ago',
    logoDetails: {
      backgroundColor: '464649',
      text: 'RT'
    }
  },
  {
    description: 'Germany says will tell U.S. at G7 that the world needs its leadership',
    url: 'bbc.com',
    lastVisited: 'yesterday',
    logoDetails: {
      backgroundColor: '179471',
      text: 'BB'
    }
  },
  {
    description: 'Fashion manufacturer Gerry Weber is insolvent. The company wants to restructure itself',
    url: 'spiegel.de',
    lastVisited: '2 hours ago',
    logoDetails: {
      backgroundColor: 'F2800C',
      text: 'SP'
    }
  },
  {
    description: 'Roger Stone, a close confident of Trumps, has been arrested in Florida.',
    url: 'bbc.com',
    lastVisited: 'yesterday',
    logoDetails: {
      backgroundColor: '464649',
      text: 'BB'
    }
  }
]

export default class News extends React.Component {
  render() {
    return (
      <FlatList
        style={styles.list}
        data={NEWS}
        renderItem={this.renderItem}
      />
    )
  }

  renderItem = ({ item }) => (
    <NewsItem data={item} />
  )
}

class NewsItem extends React.Component {
  render() {
    const data = this.props.data;

    return (
      <View style={itemStyles.container}>
        <Icon
          width={60}
          height={60}
          logoDetails={data.logoDetails}
        />
        <View style={itemStyles.rightContainer}>
          <Text style={itemStyles.description}>{data.description}</Text>
          <Text style={itemStyles.description}>{data.url} - {data.lastVisited}</Text>
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
    paddingTop: 20
  }
});

var itemStyles = StyleSheet.create({
  container: {
    flexDirection: 'row',
    marginTop: 10,
    marginLeft: 30,
    borderRadius: 4,
    borderWidth: 1,
    borderColor: '#666666',
    padding: 10
  },
  rightContainer: {
    marginLeft: 8,
    flex: 1,
    justifyContent: 'space-between'
  },
  description: {
    fontSize: 12
  }
})
