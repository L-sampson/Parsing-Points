import { Component } from '@angular/core';
import { SportsCarouselComponent } from '../sports-carousel/sports-carousel.component';
import { MatCardModule } from '@angular/material/card';
import { MatIcon } from '@angular/material/icon';

@Component({
  selector: 'app-main-page',
  standalone: true,
  imports: [SportsCarouselComponent, MatCardModule, MatIcon],
  templateUrl: './main-page.component.html',
  styleUrl: './main-page.component.css'
})
export class MainPageComponent {

}
