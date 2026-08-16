# reinert

**Reinert** is a standalone, command-line financial analysis utility built for macOS. It evaluates past results of corporate earnings reports using a high-performance vector database model to determine whether to invest in a stock right before or after an earnings release.

This also serves as an extended demonstration of another project of mine, VectorForge.

*Please don't use this as an actual financial application. I would be thoroughly horrified if anyone actually used it as so. I made this for fun over a week, and any result the model gives should be thoroughly verified before acting on it.*

## Features

* **Earnings Impact Analysis**: Leverages historical technicals, track records, valuations, and macroeconomic data to evaluate expected post-earnings stock price movement.
* **Vector-Based Similarity Matching**: Utilizes **VectorForge** to map multi-dimensional financial setups and query the nearest historical parallels.
* **Automated Data Retrieval**: Integrates with Yahoo Finance and the SEC EDGAR API to pull fresh fundamental reporting data and market charts.

## Installation

Because Reinert is built specifically as a standalone application for **macOS**, you can clone or place the binary directly into your environment and configure your working directories.

1. Ensure you have `libcurl` available on your macOS system (typically provided natively or via Homebrew via `brew install curl`).
2. Set up your workspace directory containing your model binaries and CIK mapping files.

Download and install the `reinert.zip` package and unzip it.

## Configuration (`CONFIGURE EDIT`)

When launching the application for the first time or if configuration files are missing, Reinert enters **Configure Mode**. You must define your core system paths using the `CONFIGURE EDIT` command before running analytical queries.

```text
CONFIGURE EDIT <cik_map_path> <graph_model_path> <search_profiles_path>
```

Where `cik_map_path` is the path to the `tickers.json` file, `graph_model_path` is the path to the `earnings_model.bin` file, and `search_profiles_path` is the path to the directory where you want to store the search profiles. *Ensure this directory is completely empty.*

## What Next?
Type `QUIT` and press enter and reopen the application. Go to the `/docs` folder to learn how to interact with the model and make your own if you want to.

*Again, this is not an actual financial application, make sure to completely vet whatever result the model gives.*