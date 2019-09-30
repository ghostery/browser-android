// testing data declation
<<<<<<< HEAD
const defaultMethods = [{
  supportedMethods: "basic-card",
  data: {
    supportedNetworks: ['unionpay', 'visa', 'mastercard', 'amex', 'discover',
                        'diners', 'jcb', 'mir',
    ],
||||||| merged common ancestors
const defaultMethods = [{
  supportedMethods: "basic-card",
  data: {
    supportedNetworks: ['unionpay', 'visa', 'mastercard', 'amex', 'discover',
                        'diners', 'jcb', 'mir',
    ],
    supportedTypes: ['prepaid', 'debit', 'credit'],
=======
const defaultMethods = [
  {
    supportedMethods: "basic-card",
    data: {
      supportedNetworks: [
        "unionpay",
        "visa",
        "mastercard",
        "amex",
        "discover",
        "diners",
        "jcb",
        "mir",
      ],
    },
  },
  {
    supportedMethods: "testing-payment-method",
>>>>>>> upstream-releases
  },
];

const defaultDetails = {
  total: {
    label: "Total",
    amount: {
      currency: "USD",
      value: "1.00",
    },
  },
  shippingOptions: [
    {
      id: "NormalShipping",
      label: "NormalShipping",
      amount: {
        currency: "USD",
        value: "10.00",
      },
      selected: false,
    },
    {
      id: "FastShipping",
      label: "FastShipping",
      amount: {
        currency: "USD",
        value: "5.00",
      },
      selected: false,
    },
  ],
};

const defaultOptions = {
  requestPayerName: true,
  requestPayerEmail: false,
  requestPayerPhone: false,
  requestShipping: true,
  shippingType: "shipping",
};
