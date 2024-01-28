import * as THREE from "three";
import { OrbitControls } from "three/addons/controls/OrbitControls.js";
import { GLTFLoader } from "three/addons/loaders/GLTFLoader.js";
import { OutlineEffect } from "three/examples/jsm/effects/OutlineEffect.js";
import GSAP from "gsap";
import { Pane } from "tweakpane";

// Function to get the last move from the server
// setInterval(async () => {
//   try {
//     const response = await fetch("http://localhost:3000/last-moves");
//     const data = await response.json();
//     const move = data.move ?? [undefined, undefined];
//   } catch (err) {
//     console.log(err);
//   }
// }, 1000);

// Pane
const pane = new Pane({
  container: document.getElementById("pane"),
});
const tab = pane.addTab({
  pages: [{ title: "Hand" }, { title: "Colors" }],
});
const clench = tab.pages[0].addFolder({
  title: "Clench",
  expanded: false,
});
const spread = tab.pages[0].addFolder({
  title: "Spread (WIP)",
  expanded: false,
});
const DEFAULTS = {
  bg: 0x4b46b2,
  hand: 0xd88a69,
  shirt: 0xffffff,
  vest: 0xff2727,
  wrist: 0,
  thumb: 0,
  index: 0,
  middle: 0,
  ring: 0,
  pinky: 0,
  thumbz: -0.15,
  indexz: -0.3,
  middlez: -0.08,
  ringz: -0.22,
  pinkyz: -0.52,
};

// Buttons
const rock = document.getElementById("rock");
const paper = document.getElementById("paper");
const scissors = document.getElementById("scissors");

// Canvas
const canvas = document.querySelector("canvas.webgl");

// Scene
const scene = new THREE.Scene();
const bgColor = new THREE.Color(DEFAULTS.bg);
scene.background = bgColor;

tab.pages[1]
  .addInput(DEFAULTS, "bg", {
    view: "color",
    picker: "inline",
    expanded: false,
  })
  .on("change", (ev) => {
    scene.background = new THREE.Color(ev.value);
    document.body.style.backgroundColor = ev.value;
  });

/**
 * Model
 */
const gltfLoader = new GLTFLoader();

gltfLoader.load("hand.glb", (gltf) => {
  scene.add(gltf.scene.children[0]);

  setMaterials();
  setBones();
});

// Materials
const handMaterial = new THREE.MeshToonMaterial();
const shirtMaterial = new THREE.MeshToonMaterial();
const vestMaterial = new THREE.MeshToonMaterial();

/**
 * Function that sets the materials of the model
 */
const setMaterials = () => {
  const textureLoader = new THREE.TextureLoader();
  const gradientTexture = textureLoader.load("3.jpg");
  gradientTexture.minFilter = THREE.NearestFilter;
  gradientTexture.magFilter = THREE.NearestFilter;
  gradientTexture.generateMipmaps = false;

  handMaterial.color = new THREE.Color(DEFAULTS.hand);
  handMaterial.gradientMap = gradientTexture;
  handMaterial.roughness = 0.7;
  handMaterial.emissive = new THREE.Color(DEFAULTS.hand);
  handMaterial.emissiveIntensity = 0.2;
  scene.getObjectByName("Hand").material = handMaterial;

  shirtMaterial.color = new THREE.Color(DEFAULTS.shirt);
  shirtMaterial.gradientMap = gradientTexture;
  scene.getObjectByName("Shirt").material = shirtMaterial;

  vestMaterial.color = new THREE.Color(DEFAULTS.vest);
  vestMaterial.gradientMap = gradientTexture;
  scene.getObjectByName("Vest").material = vestMaterial;

  // Pane
  tab.pages[1]
    .addInput(DEFAULTS, "hand", {
      view: "color",
      picker: "inline",
      expanded: false,
    })
    .on("change", (ev) => {
      handMaterial.color = new THREE.Color(ev.value);
      handMaterial.emissive = new THREE.Color(DEFAULTS.hand);
    });
  tab.pages[1]
    .addInput(DEFAULTS, "shirt", {
      view: "color",
      picker: "inline",
      expanded: false,
    })
    .on("change", (ev) => {
      shirtMaterial.color = new THREE.Color(ev.value);
    });
  tab.pages[1]
    .addInput(DEFAULTS, "vest", {
      view: "color",
      picker: "inline",
      expanded: false,
    })
    .on("change", (ev) => {
      vestMaterial.color = new THREE.Color(ev.value);
    });
};
/**
 * Function that sets the bones of the model
 * and adds the inputs to the pane to control them
 *
 */
const setBones = () => {
  const wrist = scene.getObjectByName("Hand").skeleton.bones[0];
  const wrist1 = scene.getObjectByName("Hand").skeleton.bones[1];
  const wrist2 = scene.getObjectByName("Hand").skeleton.bones[2];
  const wrist3 = scene.getObjectByName("Hand").skeleton.bones[6];
  const wrist4 = scene.getObjectByName("Hand").skeleton.bones[10];
  const wrist5 = scene.getObjectByName("Hand").skeleton.bones[14];
  const wrist6 = scene.getObjectByName("Hand").skeleton.bones[18];
  wrist1.rotation.x = DEFAULTS.wrist;
  wrist2.rotation.x = DEFAULTS.wrist;
  wrist3.rotation.x = DEFAULTS.wrist;
  wrist4.rotation.x = DEFAULTS.wrist;
  wrist5.rotation.x = DEFAULTS.wrist;
  wrist6.rotation.x = DEFAULTS.wrist;

  const thumb1 = scene.getObjectByName("Hand").skeleton.bones[3];
  const thumb2 = scene.getObjectByName("Hand").skeleton.bones[4];
  const thumb3 = scene.getObjectByName("Hand").skeleton.bones[5];
  thumb1.rotation.x = DEFAULTS.thumb;
  thumb2.rotation.x = DEFAULTS.thumb;
  thumb3.rotation.x = DEFAULTS.thumb;
  thumb1.rotation.z = DEFAULTS.thumbz;
  thumb2.rotation.z = DEFAULTS.thumbz;
  thumb3.rotation.z = DEFAULTS.thumbz;

  const index1 = scene.getObjectByName("Hand").skeleton.bones[7];
  const index2 = scene.getObjectByName("Hand").skeleton.bones[8];
  const index3 = scene.getObjectByName("Hand").skeleton.bones[9];
  index1.rotation.x = DEFAULTS.index;
  index2.rotation.x = DEFAULTS.index;
  index3.rotation.x = DEFAULTS.index;

  const middle1 = scene.getObjectByName("Hand").skeleton.bones[11];
  const middle2 = scene.getObjectByName("Hand").skeleton.bones[12];
  const middle3 = scene.getObjectByName("Hand").skeleton.bones[13];
  middle1.rotation.x = DEFAULTS.middle;
  middle2.rotation.x = DEFAULTS.middle;
  middle3.rotation.x = DEFAULTS.middle;

  const ring1 = scene.getObjectByName("Hand").skeleton.bones[15];
  const ring2 = scene.getObjectByName("Hand").skeleton.bones[16];
  const ring3 = scene.getObjectByName("Hand").skeleton.bones[17];
  ring1.rotation.x = DEFAULTS.ring;
  ring2.rotation.x = DEFAULTS.ring;
  ring3.rotation.x = DEFAULTS.ring;

  const pinky1 = scene.getObjectByName("Hand").skeleton.bones[19];
  const pinky2 = scene.getObjectByName("Hand").skeleton.bones[20];
  const pinky3 = scene.getObjectByName("Hand").skeleton.bones[21];
  pinky1.rotation.x = DEFAULTS.pinky;
  pinky2.rotation.x = DEFAULTS.pinky;
  pinky3.rotation.x = DEFAULTS.pinky;

  // PANE
  // Wrist
  clench
    .addInput(DEFAULTS, "wrist", { min: -0.4, max: 0.4, step: 0.01 })
    .on("change", (ev) => {
      wrist.rotation.x = ev.value;
      wrist1.rotation.x = ev.value;
      wrist2.rotation.x = ev.value;
      wrist3.rotation.x = ev.value;
      wrist4.rotation.x = ev.value;
      wrist5.rotation.x = ev.value;
      wrist6.rotation.x = ev.value;
    });

  // Thumb
  clench
    .addInput(DEFAULTS, "thumb", { min: 0, max: 0.9, step: 0.01 })
    .on("change", (ev) => {
      thumb1.rotation.x = ev.value;
      thumb2.rotation.x = ev.value;
      thumb3.rotation.x = ev.value;
    });

  spread
    .addInput(DEFAULTS, "thumbz", { min: -0.4, max: 0.3, step: 0.01 })
    .on("change", (ev) => {
      thumb1.rotation.z = ev.value;
      thumb2.rotation.z = ev.value;
      thumb3.rotation.z = ev.value;
    });

  // Index
  clench
    .addInput(DEFAULTS, "index", { min: 0, max: 1.1, step: 0.01 })
    .on("change", (ev) => {
      index1.rotation.x = ev.value;
      index2.rotation.x = ev.value;
      index3.rotation.x = ev.value;
    });

  spread
    .addInput(DEFAULTS, "indexz", { min: -0.5, max: 0, step: 0.01 })
    .on("change", (ev) => {
      index1.rotation.z = ev.value;
    });

  // Middle
  clench
    .addInput(DEFAULTS, "middle", { min: 0, max: 1.25, step: 0.01 })
    .on("change", (ev) => {
      middle1.rotation.x = ev.value;
      middle2.rotation.x = ev.value;
      middle3.rotation.x = ev.value;
    });

  spread
    .addInput(DEFAULTS, "middlez", { min: -0.35, max: 0.25, step: 0.01 })
    .on("change", (ev) => {
      middle1.rotation.z = ev.value;
    });

  // Ring
  clench
    .addInput(DEFAULTS, "ring", { min: 0, max: 1.25, step: 0.01 })
    .on("change", (ev) => {
      ring1.rotation.x = ev.value;
      ring2.rotation.x = ev.value;
      ring3.rotation.x = ev.value;
    });

  spread
    .addInput(DEFAULTS, "ringz", { min: -0.4, max: 0.2, step: 0.01 })
    .on("change", (ev) => {
      wrist5.position.x + ev.value * 0.1;
      wrist5.position.y - ev.value * 0.1;
      ring1.rotation.z = -ev.value;
    });

  // Pinky
  clench
    .addInput(DEFAULTS, "pinky", { min: 0, max: 1.15, step: 0.01 })
    .on("change", (ev) => {
      pinky1.rotation.x = ev.value;
      pinky2.rotation.x = ev.value;
      pinky3.rotation.x = ev.value;
    });

  spread
    .addInput(DEFAULTS, "pinkyz", { min: -0.52, max: -0.25, step: 0.01 })
    .on("change", (ev) => {
      wrist6.position.x + ev.value * 0.1;
      pinky1.rotation.z = -ev.value;
    });

  /**
   * Poses
   */

  const wristRotation = [
    wrist.rotation,
    wrist1.rotation,
    wrist2.rotation,
    wrist3.rotation,
    wrist4.rotation,
    wrist5.rotation,
    wrist6.rotation,
  ];
  const thumbRotation = [thumb1.rotation, thumb2.rotation, thumb3.rotation];
  const indexRotation = [index1.rotation, index2.rotation, index3.rotation];
  const middleRotation = [middle1.rotation, middle2.rotation, middle3.rotation];
  const ringRotation = [ring1.rotation, ring2.rotation, ring3.rotation];
  const pinkyRotation = [pinky1.rotation, pinky2.rotation, pinky3.rotation];

  rock.addEventListener("click", () => {
    const tlRaisedHand = GSAP.timeline();

    tlRaisedHand
      .to(
        DEFAULTS,
        {
          duration: 0.5,
          bg: 0x4b46b2,
          hand: 0xd88a69,
          shirt: 0xffffff,
          vest: 0xff2727,
          wrist: 0,
          thumb: 1,
          index: 1,
          middle: 1,
          ring: 1,
          pinky: 1,
          thumbz: -0.15,
          indexz: -0.3,
          middlez: -0.08,
          ringz: -0.22,
          pinkyz: -0.52,
        },
        "same"
      )
      .to(thumbRotation, { duration: 0.5, x: 1 }, "same")
      .to(indexRotation, { duration: 0.5, x: 1 }, "same")
      .to(middleRotation, { duration: 0.5, x: 1 }, "same")
      .to(ringRotation, { duration: 0.5, x: 1 }, "same")
      .to(pinkyRotation, { duration: 0.5, x: 1 }, "same")

      .call(() => {
        pane.refresh();
      })
      .play();
  });

  paper.addEventListener("click", () => {
    // reset the Default values
    const tlRaisedHand = GSAP.timeline();

    tlRaisedHand
      .to(
        DEFAULTS,
        {
          duration: 0,
          bg: 0x4b46b2,
          hand: 0xd88a69,
          shirt: 0xffffff,
          vest: 0xff2727,
          wrist: 0,
          thumb: 0,
          index: 0,
          middle: 0,
          ring: 0,
          pinky: 0,
          thumbz: -0.15,
          indexz: -0.3,
          middlez: -0.08,
          ringz: -0.22,
          pinkyz: -0.52,
        },
        "same"
      )
      .to(thumbRotation, { duration: 0.5, x: 0 }, "same")
      .to(indexRotation, { duration: 0.5, x: 0 }, "same")
      .to(middleRotation, { duration: 0.5, x: 0 }, "same")
      .to(ringRotation, { duration: 0.5, x: 0 }, "same")
      .to(pinkyRotation, { duration: 0.5, x: 0 }, "same")
      .call(() => {
        pane.refresh();
      })
      .play();
  });

  scissors.addEventListener("click", () => {
    // reset the Default values
    const tlRaisedHand = GSAP.timeline();

    tlRaisedHand
      .to(
        DEFAULTS,
        {
          duration: 0,
          bg: 0x4b46b2,
          hand: 0xd88a69,
          shirt: 0xffffff,
          vest: 0xff2727,
          wrist: 0,
          thumb: 1,
          index: 0,
          middle: 0,
          ring: 1,
          pinky: 1,
          thumbz: -0.15,
          indexz: -0.3,
          middlez: -0.08,
          ringz: -0.22,
          pinkyz: -0.52,
        },
        "same"
      )
      .to(thumbRotation, { duration: 0.5, x: 1 }, "same")
      .to(indexRotation, { duration: 0.5, x: 0 }, "same")
      .to(middleRotation, { duration: 0.5, x: 0 }, "same")
      .to(ringRotation, { duration: 0.5, x: 1 }, "same")
      .to(pinkyRotation, { duration: 0.5, x: 1 }, "same")
      .call(() => {
        pane.refresh();
      })
      .play();
  });
};

/**
 * Lights
 */
const ambientLight = new THREE.AmbientLight(0xffffff, 0.2);
scene.add(ambientLight);

const directionalLight = new THREE.DirectionalLight(0xffffff, 2);
directionalLight.position.set(-5, 5, 5);
directionalLight.scale.set(0.5, 0.5, 0.5);
scene.add(directionalLight);

/**
 * Sizes
 */
const sizes = {
  width: window.innerWidth / 2,
  height: window.innerHeight,
};

window.addEventListener("resize", () => {
  // Update sizes
  sizes.width = window.innerWidth / 2;
  sizes.height = window.innerHeight;

  // Update camera
  camera.aspect = sizes.width / sizes.height;
  camera.updateProjectionMatrix();

  // Update renderer
  outlineEffect.setSize(sizes.width, sizes.height);
  outlineEffect.setPixelRatio(Math.min(window.devicePixelRatio, 2));
});

/**
 * Camera
 */
const camera = new THREE.PerspectiveCamera(
  75,
  sizes.width / sizes.height,
  0.1,
  100
);
camera.position.set(0, 0, 5);
scene.add(camera);

// Controls
const controls = new OrbitControls(camera, canvas);
controls.target.set(0, 0, 0);
controls.enableDamping = true;
controls.maxPolarAngle = Math.PI / 2;
controls.minDistance = 3;
controls.maxDistance = 10;

/**
 * Renderer
 */
const renderer = new THREE.WebGLRenderer({
  canvas: canvas,
  alpha: true,
});
renderer.shadowMap.enabled = true;
renderer.shadowMap.type = THREE.PCFSoftShadowMap;
renderer.setSize(sizes.width, sizes.height);
renderer.setPixelRatio(Math.min(window.devicePixelRatio, 2));

const outlineEffect = new OutlineEffect(renderer, {
  defaultThickness: 0.0035,
  defaultColor: [0, 0, 0],
  defaultAlpha: 0.8,
  defaultKeepAlive: true,
});

/**
 * Animate
 */
const clock = new THREE.Clock();
let previousTime = 0;

const tick = () => {
  const elapsedTime = clock.getElapsedTime();
  const deltaTime = elapsedTime - previousTime;
  previousTime = elapsedTime;

  // Update controls
  controls.update();

  // Render
  outlineEffect.render(scene, camera);

  // Call tick again on the next frame
  window.requestAnimationFrame(tick);
};

tick();

