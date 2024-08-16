import { Routes } from '@angular/router';
import { SportsListComponent } from './components/sports-list/sports-list.component';
import { ScoresListComponent } from './components/scores-list/scores-list.component';
import { ScoreboardComponent } from './components/scoreboard/scoreboard.component';
import { OddsListComponent } from './components/odds-list/odds-list.component';

export const routes: Routes = [
    {path: 'sports', component: SportsListComponent}, 
    {path: 'scores', component: ScoresListComponent},
    {path: 'scoreboard', component: ScoreboardComponent},
    {path: 'odds', component: OddsListComponent}
];
