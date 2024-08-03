import { Component, ViewChild } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { NgbCarousel, NgbCarouselModule, NgbSlideEvent, NgbSlideEventSource  } from '@ng-bootstrap/ng-bootstrap';

@Component({
  selector: 'app-sports-carousel',
  standalone: true,
  imports: [FormsModule, NgbCarouselModule],
  templateUrl: './sports-carousel.component.html',
  styleUrl: './sports-carousel.component.css'
})
export class SportsCarouselComponent {
  image_names: string[] = [
    "baseball-stadium.jpg",
    "basketball.jpg",
    "betting-table.jpg",
    "hockey.jpg",
    "Parsing-points-logo.png",
    "soccer-stadium.jpg",
    "soccer-team.jpg", 
    "football.jpg",
    "watch-games.jpg"
 ].map((index) => `assets/sport-images/${index}`)

  paused = false;
  unpauseOnArrow = false;
  pauseOnIdicator = false;
  pauseOnHover = true;
  pauseOnFocus = true;

  @ViewChild('carousel', {static: true}) carousel!: NgbCarousel;

  togglePaused() {
    if(this.paused) {
      this.carousel.cycle();
    } else {
      this.carousel.pause();
    }
    this.paused != this.paused;
  }

  onSlide(slideEvent: NgbSlideEvent) {
    if(this.unpauseOnArrow && slideEvent.paused && (slideEvent.source == NgbSlideEventSource.ARROW_LEFT || slideEvent.source == NgbSlideEventSource.ARROW_RIGHT)) {
      this.togglePaused();
    }
    if(this.pauseOnIdicator && !slideEvent.paused && slideEvent.source == NgbSlideEventSource.INDICATOR) {
      this.togglePaused();
    }
  }

  
  ngOnInit(): void {
    console.log(this.image_names);
  }


}
