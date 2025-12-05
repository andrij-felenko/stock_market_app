# stock_market_app

## Environment configuration

Sensitive API keys are now expected to be provided via environment variables. Copy `.env.example` to `.env` and fill in your real credentials:

```bash
cp .env.example .env
```

The application reads these variables at startup:

```
ALPHAVANTAGE_API_KEY=
EOD_API_KEY=
FINNHUB_API_KEY=
MARKETSTACK_API_KEY=
TWELVEDATA_API_KEY=
FIGI_API_KEY=
OPENAI_API_KEY=
STOCK_SERVICE_URL=
```

Never commit the populated `.env` file to version control.