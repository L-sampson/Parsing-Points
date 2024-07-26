import { Routes } from '@angular/router';
import { SportsListComponent } from './components/sports-list/sports-list.component';
import { ScoresListComponent } from './components/scores-list/scores-list.component';

export const routes: Routes = [
    {path: 'sports', component: SportsListComponent}, 
    {path: 'scores', component: ScoresListComponent}
];
