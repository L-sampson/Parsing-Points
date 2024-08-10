import { CommonModule } from '@angular/common';
import { Component } from '@angular/core';
import { MatExpansionModule } from '@angular/material/expansion';
import { HttpClient, HttpClientModule } from '@angular/common/http';
import { Observable, of } from 'rxjs';
import { map, tap, timestamp } from 'rxjs/operators';
import { ScoresData } from '../../interfaces/scores';
import { MatCardModule } from '@angular/material/card';
import { MatButtonModule } from '@angular/material/button';
import { environment } from '../../../environments/environment';
import { ScoreboardComponent } from '../scoreboard/scoreboard.component';
import {MatSidenavModule} from '@angular/material/sidenav';
import { MatIcon } from '@angular/material/icon';

@Component({
  selector: 'app-scores-list',
  standalone: true,
  imports: [
    MatExpansionModule, CommonModule, HttpClientModule, 
    MatCardModule, MatButtonModule, ScoreboardComponent, 
    MatSidenavModule, MatIcon],
  templateUrl: './scores-list.component.html',
  styleUrls: ['./scores-list.component.css']
})
export class ScoresListComponent {
  sportScoreMap = new Map<string, Set<ScoresData>>([
    ["mlb", new Set()],
  ]);

   sportsLeagueMap = new Map<string, string>(
    [
      ["mlb", ''],
      ['nba', ''],
      ['wnba', ''],
      ['mls', ''], 
      ['nhl', '']
    ]);

  private logoCache = new Map<string, {logo: string, timestamp: number}>;

  constructor(private http: HttpClient) {}

  ngOnInit(): void {
    this.getSportMatchData();
    this.fetchLeagueLogos();
  }

  fetchLeagueLogos() {
    this.sportsLeagueMap.forEach((_, league)=>{
      this.http.get<any>(`https://www.thesportsdb.com/api/v2/json/3/search/league/${league}`)
      .pipe(map(data => data.leagues[0].strBadge))
      .subscribe(logo => {
        this.sportsLeagueMap.set(league, logo)
      })
    })
  }

  getSportScores(sportParam: string): Observable<ScoresData[]> {
    const url = `${environment.API_URL}/scores/${sportParam}`;
    console.log('Fetching scores from URL:', url);
    return this.http.get<ScoresData[]>(url);
  }

  fetchTeamLogo(teamName: string): Observable<string> {
    const cached = this.logoCache.get(teamName);
    const now = Date.now();
    const oneDay = 24 * 60 * 60 * 1000;
    
    if (cached && (now - cached.timestamp) < oneDay) {
      console.log(`Fetching logo for ${teamName} from cache`);
      return of(cached.logo);
    }

    const trimmedTeamName: string = teamName.replace(/ /g, "_");
    return this.http.get<any>(`https://www.thesportsdb.com/api/v1/json/3/searchteams.php?t=${trimmedTeamName}`).pipe(
      map(data => data.teams[0].strBadge),
      tap(logo => this.logoCache.set(teamName, {logo, timestamp: now}))
    );
  }

  getSportMatchData(): void {  
    this.sportScoreMap.forEach((_, sport) => {
      this.getSportScores(sport).subscribe(matches => {
        matches.forEach((game: ScoresData) => {
          console.log(`Home Team: ${game.home_team}`);
          console.log(`Away Team: ${game.away_team}`);
          const sportSet = this.sportScoreMap.get(sport) || new Set<ScoresData>();
          sportSet.add(game);
          this.sportScoreMap.set(sport, sportSet);
          console.log(this.sportScoreMap)
          this.fetchTeamLogo(game.home_team).subscribe(logo => {
            game.home_team_logo = logo;
          })
          this.fetchTeamLogo(game.away_team).subscribe(logo => {
            game.away_team_logo = logo;
          })
        });
      });
    });
  }
  
}
