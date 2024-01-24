export default function Hand() {
  return (
    <div className="flex min-h-screen flex-col items-center justify-between p-24">
      <div id="pane"></div>
      <div id="buttons">
        <button id="raised-hand">Raised Hand</button>
        <button id="raised-finger">Raised Finger</button>
        <button id="rock-on">Rock On</button>
        <button id="peace">Peace</button>
        <button id="hang-loose">Hang Loose</button>
        <button id="fu">FU</button>
        <button id="vulcan-salute">Vulcan Salute</button>
      </div>
      <canvas className="webgl"></canvas>
    </div>
  );
}
