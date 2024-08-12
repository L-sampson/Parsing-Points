import { CommonModule } from '@angular/common';
import { Component } from '@angular/core';
import { MatExpansionModule } from '@angular/material/expansion';
import { HttpClient, HttpClientModule } from '@angular/common/http';
import { forkJoin, Observable, of } from 'rxjs';
import { map, switchMap, tap, timestamp } from 'rxjs/operators';
import { ScoresData } from '../../interfaces/scores';
import { EventOdds } from '../../interfaces/odds';
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
    ["nba", new Set()],
    ["wnba", new Set()],
    ["mls", new Set()],
    ["nhl", new Set()]
  ]);

  leagueLogoMap = new Map<string, string>(
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
    this.fetchLeagueLogos();
  }

  fetchLeagueLogos() {
    this.sportScoreMap.forEach((_, league)=>{
      this.http.get<any>(`https://www.thesportsdb.com/api/v2/json/3/search/league/${league}`)
      .pipe(map(data => data.leagues[0].strBadge))
      .subscribe(logo => {
        this.leagueLogoMap.set(league, logo)
      })
    })
  }

  fetchEventOdds(sportParam: string): Observable<EventOdds[]> {
    const url = `${environment.API_URL}/odds?sport=${sportParam}&market=h2h&region=us`;
    console.log('Fetching scores from URL:', url);
    return this.http.get<EventOdds[]>(url);
  }

  getSportScores(sportParam: string): Observable<ScoresData[]> {
    const url = `${environment.API_URL}/scores?sport=${sportParam}`;
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

  selectedLeague: string  | null = null;

  combineScoresWithOdds(scores: ScoresData[], odds: EventOdds[]) : ScoresData [] {
    const oddsMap = new Map<string, EventOdds>();
    odds.forEach(odd => oddsMap.set(odd.id, odd));

    return scores.map(score => {
      const matchOdds = oddsMap.get(score.id);
      if(matchOdds) {
        score.eventOdds = {
          id: matchOdds.id,
          home_team: matchOdds.home_team,
          away_team: matchOdds.away_team,
          commence_time: matchOdds.commence_time,
          bookmakers: matchOdds.bookmakers.length > 0 ? [matchOdds.bookmakers[0]] : []
        }
      }
      return score;
    });
  }

  showLeagueScores(leagueName: string): void {
    if (this.sportScoreMap.has(leagueName)) {
      console.log("Found league name: ", leagueName);
      this.selectedLeague = leagueName;
  
      forkJoin({
        odds: this.fetchEventOdds(leagueName),
        scores: this.getSportMatchData()
      }).pipe(
        map(({ odds, scores }) => {
          return this.combineScoresWithOdds(scores, odds);
        })
      ).subscribe(combinedScores => {
        console.log("Combined scores with odds:", combinedScores);
        console.log("Event odds: ", combinedScores[0]?.eventOdds?.bookmakers[0]?.key);
        this.sportScoreMap.set(leagueName, new Set(combinedScores));
      });
    } else {
      console.log("Could not find league.");
    }
  }
  

  isPanelVisible(leaguePanel: string): boolean {
    return this.selectedLeague === leaguePanel;
  }


  getSportMatchData(): Observable<ScoresData[]> {
    if (this.selectedLeague) {
      console.log("Fetching scores from league", this.selectedLeague);
  
      return this.getSportScores(this.selectedLeague).pipe(
        switchMap(matches => {
          const sportSet = this.sportScoreMap.get(this.selectedLeague!) || new Set<ScoresData>();
          const leagueLogo = this.leagueLogoMap.get(this.selectedLeague!) || '';
          
          const logoRequests = matches.map((game: ScoresData) => {
            game.league_logo = leagueLogo;
            sportSet.add(game);
  
            return forkJoin([
              this.fetchTeamLogo(game.home_team).pipe(map(logo => game.home_team_logo = logo)),
              this.fetchTeamLogo(game.away_team).pipe(map(logo => game.away_team_logo = logo))
            ]).pipe(map(() => game));
          });
  
          return forkJoin(logoRequests).pipe(
            map(updatedGames => {
              this.sportScoreMap.set(this.selectedLeague!, new Set(updatedGames));
              return updatedGames;
            })
          );
        })
      );
    } else {
      console.log("Error: No selected league.");
      return of([]);
    }
  }
  
}
