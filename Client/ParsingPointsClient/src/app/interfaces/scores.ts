export interface Scores {
    away_team: string
    commence_time: string
    completed: boolean,
    home_team: string
    // id: string
    // last_update: string
    scoreboard?: ScoreBoard []
    // sport_key: string
    // sport_title: string
}

export interface ScoreBoard {
    name: string
    score: string
}
