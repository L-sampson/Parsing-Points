import { Component, OnInit } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { MatCardModule } from '@angular/material/card';
import { MatTableModule } from '@angular/material/table';
import { CommonModule } from '@angular/common';
import { environment } from '../../../environments/environment';
import { Observable } from 'rxjs';

interface Outcome {
  name: string;
  price: number;
  point?: number;
}

interface Market {
  key: string;
  outcomes: Outcome[];
}

interface Bookmaker {
  key: string;
  title: string;
  markets: Market[];
}

interface Game {
  home_team: string;
  away_team: string;
  bookmakers: Bookmaker[];
}

interface TransformedData {
  name: string;
  win: number;
  spread: string;
}

@Component({
  selector: 'app-odds-list',
  standalone: true,
  imports: [MatCardModule, MatTableModule, CommonModule],
  templateUrl: './odds-list.component.html',
  styleUrls: ['./odds-list.component.css']
})
export class OddsListComponent implements OnInit {
  constructor(private http: HttpClient) {}

  displayedColumns: string[] = ['name', 'win', 'spread'];
  dataSource: Game[] = [];

  

  ngOnInit(): void {
    this.fetchUpcomingOdds();
  }

  fetchUpcomingOdds() {
    const url = 'assets/odds.json';
    this.http.get<Game[]>(url).subscribe(gameOddData => {
      this.dataSource = gameOddData;
      console.log('This game odd Data', gameOddData);
    })
  }

  transformGameData(game: Game): TransformedData[] {
    const firstBookmaker = game.bookmakers[0];
    const win = firstBookmaker.markets.find(m => m.key === 'h2h')?.outcomes || [];
    const spreads = firstBookmaker.markets.find(m => m.key === 'spreads')?.outcomes || [];

    return win.map(w => {
      const spread = spreads.find(s => s.name === w.name);
      return {
        name: w.name,
        win: w.price,
        spread: spread ? `${spread.point} (${spread.price})` : '',
      };
    });
  }
}
