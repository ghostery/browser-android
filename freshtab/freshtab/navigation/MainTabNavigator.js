import React from 'react';
import { Platform, Image, StyleSheet } from 'react-native';
import { createMaterialTopTabNavigator } from 'react-navigation';

import HomeScreen from '../screens/HomeScreen';
import News from '../screens/News';
import Bikini from '../screens/Bikini';
import Favorites from '../screens/Favorites';
import NativeDrawable from '../components/NativeDrawable';


const tabNavigatorConfig = {
  tabBarOptions: {
    style: {
      backgroundColor: '#000000'
    },
    labelStyle: {
      fontSize: 18
    },
    showLabel: false,
    showIcon: true,
    activeTintColor: '#00AEF0',
    inactiveTintColor: '#ace8ff',
    indicatorStyle: {
      backgroundColor: '#000',
      borderBottomWidth: 2,
      borderBottomColor: '#00AEF0'
    },
    tabStyle: {
      borderBottomWidth: 1,
      borderBottomColor: '#CCCCCC',
    }
  }
}

export default createMaterialTopTabNavigator({
  Home: {
    screen: HomeScreen,
    navigationOptions: ({ navigation }) => ({
      title: "Home",
      tabBarIcon: ({tintColor}) => (
        <NativeDrawable
          source='ic_home_cliqz'
          color={tintColor}
          style={{width: 30, height: 30}}
        />

      )
		})
  },
  News: {
    screen: News,
    navigationOptions: ({ navigation }) => ({
      title: 'News',
      tabBarIcon: ({tintColor}) => (
        <NativeDrawable
          source='ic_news'
          color={tintColor}
          style={{width: 30, height: 30}}
        />
      )
    }),
  },
  Bikini: {
    screen: Bikini,
    navigationOptions: ({ navigation }) => ({
      title: 'Bikini',
      tabBarIcon: ({tintColor}) => (
        <NativeDrawable
          source='ic_bikini'
          color={tintColor}
          style={{width: 30, height: 30}}
        />

      )
    }),
  },
  Favorites: {
    screen: Favorites,
    navigationOptions: ({ navigation }) => ({
      title: 'Fourth',
      tabBarIcon: ({tintColor, focused} = {}) => {
        console.log(tintColor)
        return (
        <NativeDrawable
          source='ic_favorites'
          style={{width: 25, height: 25 }}
          color={tintColor}
        />

      )}
    }),
  }

}, tabNavigatorConfig);
