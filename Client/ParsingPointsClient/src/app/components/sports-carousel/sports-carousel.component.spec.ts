import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SportsCarouselComponent } from './sports-carousel.component';

describe('SportsCarouselComponent', () => {
  let component: SportsCarouselComponent;
  let fixture: ComponentFixture<SportsCarouselComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [SportsCarouselComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(SportsCarouselComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
