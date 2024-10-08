export interface EventOdds {
    id: string
    commence_time: string
    home_team: string
    away_team: string
    bookmakers: Bookmaker[]
}

export interface Bookmaker {
    key: string
    title: string
    last_update: string
    markets: Market[]
}

export interface Market {
    key: string
    last_update: string
    outcomes: Outcome[]
}

export interface Outcome {
    name: string
    price: number
}
