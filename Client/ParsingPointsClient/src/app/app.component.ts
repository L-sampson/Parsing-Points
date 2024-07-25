import { CommonModule } from '@angular/common';
import { HttpClient, HttpClientModule } from '@angular/common/http';
import { Component } from '@angular/core';
import { RouterOutlet } from '@angular/router';
import { HeaderComponent } from "./components/header/header.component";
import { SportsListComponent } from './components/sports-list/sports-list.component';

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [RouterOutlet, CommonModule, HttpClientModule, HeaderComponent, SportsListComponent],
  templateUrl: './app.component.html',
  styleUrl: './app.component.css'
})
export class AppComponent {
  title = 'ParsingPointsClient';
  data: any;

  constructor(private http: HttpClient) {}

  ngOnInit(): void {
  //   this.http.get<any>('http://0.0.0.0:8080/scores/nba')
  //   .subscribe(data => {
  //   this.data = data;
  //   data.forEach((element: any) => {
  //   console.log(element);
  //   console.log("Title: ", element.title)
  //      });
  //    })
  }
}
