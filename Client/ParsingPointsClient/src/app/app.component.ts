import { CommonModule } from '@angular/common';
import { HttpClient, HttpClientModule } from '@angular/common/http';
import { Component } from '@angular/core';
import { RouterOutlet } from '@angular/router';
import { HeaderComponent } from "./components/header/header.component";
import { SportsListComponent } from './components/sports-list/sports-list.component';
import { SportsCarouselComponent } from './components/sports-carousel/sports-carousel.component';

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [RouterOutlet, CommonModule, HttpClientModule, HeaderComponent, SportsListComponent, SportsCarouselComponent],
  templateUrl: './app.component.html',
  styleUrl: './app.component.css'
})
export class AppComponent {
  title = 'ParsingPointsClient';
  data: any;

  constructor(private http: HttpClient) {}

  ngOnInit(): void {
  }
}
