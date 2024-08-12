import { Component, Input } from '@angular/core';
import { MatCardModule } from '@angular/material/card'
import { MatButtonModule } from '@angular/material/button';
import { MatIconModule } from '@angular/material/icon';
import { CommonModule } from '@angular/common';
import { ScoresData } from '../../interfaces/scores';

@Component({
  selector: 'app-scoreboard',
  standalone: true,
  imports: [MatCardModule, MatButtonModule, MatIconModule, CommonModule],
  templateUrl: './scoreboard.component.html',
  styleUrl: './scoreboard.component.css'
})
export class ScoreboardComponent {
  @Input() match: ScoresData | undefined;
  @Input() leagueName = '';

  ngOnInit() {
    console.log('Event:', this.match);
    console.log('League Name:', this.leagueName);
  }
}
