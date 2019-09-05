import { Injectable, OnDestroy } from '@angular/core';
import { Observable, Subject } from 'rxjs';
const dbus = require('dbus-next');
import { DBusDisplayBrightnessGnome } from '../../common/classes/DBusDisplayBrightnessGnome';

@Injectable({
  providedIn: 'root'
})
export class DBusService implements OnDestroy {

  private sessionBus: any;
  private dbusProperties: any;

  public observeDisplayBrightness: Observable<number>;
  private displayBrightnessSubject: Subject<number>;
  public currentDisplayBrightness: number;
  public displayBrightnessNotSupported = false;

  public displayBrightnessGnome: DBusDisplayBrightnessGnome;

  private dbusDriverNames: string[] = [];

  constructor() {
    this.displayBrightnessSubject = new Subject<number>();
    this.observeDisplayBrightness = this.displayBrightnessSubject.asObservable();

    try {
      this.sessionBus = dbus.sessionBus();

    } catch (err) {
      console.log('dbus.sessionBus() error: ', err);
      this.sessionBus = undefined;
    }

    this.initDusDisplayBrightness().then(() => {
      const driversList: string[] = [];
      if (this.displayBrightnessNotSupported === false) {
        driversList.push(this.displayBrightnessGnome.getDescriptiveString());
      }
      this.dbusDriverNames = driversList;
    });
  }

  public async initDusDisplayBrightness(): Promise<void> {
    return new Promise<void>(async resolve => {

      if (this.sessionBus === undefined) {
        this.displayBrightnessNotSupported = true;
      } else {
        this.displayBrightnessGnome = new DBusDisplayBrightnessGnome(this.sessionBus);
        if (!await this.displayBrightnessGnome.isAvailable()) {
          this.displayBrightnessNotSupported = true;
          return;
        }

        try {
          const result = await this.displayBrightnessGnome.getBrightness();
          this.currentDisplayBrightness = result;
          this.displayBrightnessSubject.next(this.currentDisplayBrightness);
        } catch (err) {
          this.displayBrightnessNotSupported = true;
          return;
        }

        this.displayBrightnessGnome.setOnPropertiesChanged(
          (value) => {
            this.currentDisplayBrightness = value;
            this.displayBrightnessSubject.next(this.currentDisplayBrightness);
          }
        );
      }
      resolve();
    });
  }

  public getDBusDriverNames(): string[] {
    return this.dbusDriverNames;
  }

  public async setDisplayBrightness(valuePercent: number): Promise<void> {
    return this.displayBrightnessGnome.setBrightness(valuePercent).catch(() => {});
  }

  ngOnDestroy() {
    this.displayBrightnessGnome.cleanUp();
  }
}
