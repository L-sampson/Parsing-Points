export interface ScoresData {
    away_team: string
    away_team_logo: string
    commence_time: string
    completed: boolean
    home_team: string
    home_team_logo: string
    league_logo: string
    // id: string
    // last_update: string
    scores?: Score []
    // sport_key: string
    // sport_title: string
}

export interface Score {
    name: string
    score: string
}
