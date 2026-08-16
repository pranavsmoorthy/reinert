# Reinert CLI Command Guide

This document outlines the available commands and grammar rules for the Reinert command-line interface. Why did I make a command-line interface? Well, I couldn't help myself.

## 1. Analysis Commands (`ANALYZE`)

The `ANALYZE` command evaluates a stock setup or custom numerical coordinate array against historical earnings reports using the vector database model.

### Ticker Mode
Queries live and fundamental data for a specific stock ticker by pulling recent chart history fom Yahoo Finance and SEC EDGAR filings. It automatically builds the feature vector using provided fundamental inputs.

```text
ANALYZE TICKER <ticker> <consecutive_eps_beats> <avg_eps_surprise> [WITH <profile_name>]
```

* **Example:** `ANALYZE TICKER AAPL 4 1.5`
* **Example with Profile:** `ANALYZE TICKER NVDA 6 2.1 WITH aggressive_profile`

### Manual Mode
Evaluates a raw, pre-scaled 12-dimensional coordinate vector directly against the graph database.
```text
ANALYZE MANUAL [<coord_1>, <coord_2>, ..., <coord_12>] [WITH <profile_name>]`
```
* **Example:** `ANALYZE MANUAL [0.12, 0.45, 0.88, 0.33, 0.50, 0.75, 0.22, 0.60, 0.40, 0.80, 0.10, 0.90]`

Each dimension represents a different trait of the stock
* **Dimension 0 (`p[0]`):** **30-Day Momentum** — Scaled between -0.5 and 0.5.
* **Dimension 1 (`p[1]`):** **2-Week RSI (Relative Strength Index)** — Clamped between 0.0 and 1.0.
* **Dimension 2 (`p[2]`):** **Distance from 50-Day SMA** — Scaled between -0.4 and 0.4.
* **Dimension 3 (`p[3]`):** **Sector ETF Momentum** — Scaled between -0.2 and 0.2.
* **Dimension 4 (`p[4]`):** **Consecutive EPS Beats** — Scaled between 0.0 and 8.0.
* **Dimension 5 (`p[5]`):** **Average EPS Surprise** — Scaled between -1.0 and 1.0.
* **Dimension 6 (`p[6]`):** **Previous Quarter Gap** — Scaled between -0.3 and 0.3.
* **Dimension 7 (`p[7]`):** **Trailing P/E Ratio** — Scaled between 0.0 and 100.0.
* **Dimension 8 (`p[8]`):** **Year-over-Year Revenue Growth** — Scaled between -0.5 and 1.0.
* **Dimension 9 (`p[9]`):** **Market Capitalization** — Log10 transformed and scaled between.
* **Dimension 10 (`p[10]`):** **VIX Level (Volatility Index)** — Scaled between 10.0 and 85.0.
* **Dimension 11 (`p[11]`):** **10-Year Treasury Yield** — Scaled between 0.0 and 10.0.

## 2. Configuration Commands (`CONFIGURE`)

The `CONFIGURE` command manages system paths, indices, and database binaries. Note that when the application lacks proper setup, it enters configuration mode, restricting execution exclusively to configuration, clearing, and quitting utilities.

### View Configuration
Displays the currently active file paths for the CIK mapping database, vector graph model, and search profiles directory.
```text
CONFIGURE VIEW
```

### Edit Configuration
Updates the required operational paths for the runtime environment.
```text
CONFIGURE EDIT <cik_map_path> <graph_model_path> <search_profiles_path>
```
* **Example:** `CONFIGURE EDIT data/cik_map.json models/earnings_model.bin profiles/`

## 3. Profile Management Commands (`PROFILE`)

Vector search profiles allow you to customize parameters such as nearest cluster/node limits and exploration factors (`ef_search`, `ef_construction`).

### Add a Profile
Creates and stores a new named search profile with custom database traversal metrics.
```text
PROFILE ADD <name> <nearest_clusters> <nearest_nodes> <clusters_ef> <node_ef>
```
* **Example:** `PROFILE ADD custom_profile 5 20 64 64`

When creating a profile using `PROFILE ADD`, the parameters configure the graph traversal behavior and vector search accuracy within VectorForge:

* **`name`:** The unique string identifier for the profile.
* **`nearest_clusters`:** The number of graph clusters to evaluate during initial coarse-grained routing.
* **`nearest_nodes`:** The number of individual nodes (historical earnings setups) to retrieve and compare in the final search pass.
* **`clusters_ef`:** The exploration factor (`ef`) for cluster search traversal, determining how thoroughly the graph's cluster layer is explored.
* **`node_ef`:** The exploration factor (`ef`) for node-level graph traversal, balancing search precision against performance speed.

### View Profiles
View either a comprehensive list of all loaded profile names or inspect the specific configuration parameters of a single profile.
* **Syntax (All):** `PROFILE VIEW ALL`
* **Syntax (One):** `PROFILE VIEW ONE <profile_name>`
* **Example:** `PROFILE VIEW ONE custom_profile`

### Remove a Profile
Deletes an existing search profile from memory and disk.
* **Syntax:** `PROFILE REMOVE <profile_name>`
* **Example:** `PROFILE REMOVE custom_profile`

## 4. General Utilities

### Clear Terminal
Clears the console screen output.
* **Syntax:** `CLEAR`

### Exit Application
Terminates the reinert application loop.
* **Syntax:** `QUIT`

## Command Chaining

The Reinert parser supports sequential command execution by chaining multiple expressions together. You can execute multiple operations in a single line or session by listing them consecutively, and the abstract syntax tree interpreter will run them in order from first to last.

### Example of Chaining:
```text
CONFIGURE VIEW ANALYZE TICKER AAPL 4 1.5 CLEAR
```