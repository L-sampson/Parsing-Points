import { Component } from '@angular/core';
import { HttpClient, HttpClientModule } from '@angular/common/http';
import { MatCardModule } from '@angular/material/card';
import { CommonModule } from '@angular/common';
import { Sports } from '../../interfaces/sports';
import { environment } from '../../../environments/environment';

@Component({
  selector: 'app-sports-list',
  standalone: true,
  imports: [HttpClientModule, MatCardModule, CommonModule],
  templateUrl: './sports-list.component.html',
  styleUrl: './sports-list.component.css'
})
export class SportsListComponent {
  sports!: Sports;
  sports_titles: Set<Sports> = new Set();

  constructor(private http: HttpClient) {}

  ngOnInit(): void {
    console.log("apiurl", (`${environment.API_URL}/sports`));
    this.http.get<any>(`${environment.API_URL}/sports`)
    .subscribe(sports => {
      this.sports = sports;
      sports.forEach((sport: any) => {
        this.sports_titles.add(sport);
        console.log(sport);
        console.log("Title: ", sport.title)
      });
    })
  }



}
