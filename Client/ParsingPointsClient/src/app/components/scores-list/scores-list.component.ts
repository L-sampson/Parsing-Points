import { CommonModule } from '@angular/common';
import { Component } from '@angular/core';
import { MatExpansionModule } from '@angular/material/expansion';
import { HttpClient, HttpClientModule } from '@angular/common/http';
import { Observable, map, from, mergeMap, reduce } from 'rxjs';
import { ScoresData} from '../../interfaces/scores';
import { MatCardModule } from '@angular/material/card';
import { MatButtonModule } from '@angular/material/button';
import { environment } from '../../../environments/environment';

@Component({
  selector: 'app-scores-list',
  standalone: true,
  imports: [MatExpansionModule, CommonModule, HttpClientModule, MatCardModule, MatButtonModule],
  templateUrl: './scores-list.component.html',
  styleUrl: './scores-list.component.css'
})
export class ScoresListComponent {
  sportScoreMap = new Map<string, Set<ScoresData>>([
    ["nba", new Set()],
    ["wnba", new Set()],
    ["mlb", new Set()],
    ["nhl", new Set()],
    ["mls", new Set()], 
    ["nrl", new Set()]
  ]);

  constructor(private http: HttpClient) {}

  ngOnInit(): void {
    this.iterSportsMap(this.sportScoreMap)
    .subscribe(updateScoreMap => {
      this.sportScoreMap = updateScoreMap
      console.log(this.sportScoreMap)
    })
  }

  getSportScores(sportParam: string): Observable<ScoresData[]> {
    const url = `${environment.API_URL}/scores/${sportParam}`;
    console.log('Fetching scores from URL:', url);  // Log the URL to the console
    return this.http.get<ScoresData[]>(url);
  }
  

  iterSportsMap(sportsMap: Map<string, Set<ScoresData>>): Observable<Map<string, Set<ScoresData>>> {
    return from(sportsMap.entries())
      .pipe(
        mergeMap(([sport, event]) =>
          this.getSportScores(sport).pipe(
            map(games => {
              games.forEach(game => {
                const gameSummary: ScoresData = {
                  home_team: game.home_team,
                  away_team: game.away_team,
                  commence_time: game.commence_time,
                  completed: game.completed,
                  scores: game.scores
                }
                event.add(gameSummary)
              });
              return sportsMap;
            })
          )
        ),
        reduce((map) => map, new Map(sportsMap))
      );
  }
  
  

}
