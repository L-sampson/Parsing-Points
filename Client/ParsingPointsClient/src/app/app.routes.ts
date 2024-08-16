import { Routes } from '@angular/router';
import { SportsListComponent } from './components/sports-list/sports-list.component';
import { ScoresListComponent } from './components/scores-list/scores-list.component';
import { ScoreboardComponent } from './components/scoreboard/scoreboard.component';
import { OddsListComponent } from './components/odds-list/odds-list.component';
import { MainPageComponent } from './components/main-page/main-page.component';
import { PageNotFoundComponent } from './components/page-not-found/page-not-found.component';

export const routes: Routes = [
    {path: '', component: MainPageComponent},
    {path: 'sports', component: SportsListComponent}, 
    {path: 'scores', component: ScoresListComponent},
    {path: 'scoreboard', component: ScoreboardComponent},
    {path: 'odds', component: OddsListComponent},
    {path: '**', component:PageNotFoundComponent}
];
