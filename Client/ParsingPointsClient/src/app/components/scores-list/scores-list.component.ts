import { CommonModule } from '@angular/common';
import { Component } from '@angular/core';
import { MatExpansionModule } from '@angular/material/expansion';
import { HttpClient, HttpClientModule } from '@angular/common/http';
import { Observable, map, from, mergeMap, reduce } from 'rxjs';
import { Scores} from '../../interfaces/scores';
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
  sportScoreMap = new Map<string, Set<Scores>>([
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

  getSportScores(sportParam: string): Observable<Scores[]> {
    return this.http.get<Scores[]>(`${environment.apiUrl}/scores/${sportParam}`)
  }

  iterSportsMap(sportsMap: Map<string, Set<Scores>>): Observable<Map<string, Set<Scores>>> {
    return from(sportsMap.entries())
      .pipe(
        mergeMap(([sport, scores]) =>
          this.getSportScores(sport).pipe(
            map(games => {
              games.forEach(game => {
                const gameSummary: Scores = {
                  home_team: game.home_team,
                  away_team: game.away_team,
                  commence_time: game.commence_time,
                  completed: game.completed,
                  scoreboard: game.scoreboard
                }
                scores.add(gameSummary)
              });
              return sportsMap;
            })
          )
        ),
        reduce((map) => map, new Map(sportsMap))
      );
  }
  
  

}
