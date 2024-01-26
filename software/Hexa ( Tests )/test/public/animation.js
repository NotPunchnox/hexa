const animatePoseBtn = document.getElementById("animatePoseBtn");

animatePoseBtn.addEventListener("click", () => {
  const pose = {
    leftFront: {
      alpha: getRandomAngle(),
          beta: getRandomAngle(),
          gamma: getRandomAngle(),
        },
        rightFront: {
          alpha: getRandomAngle(),
          beta: getRandomAngle(),
          gamma: getRandomAngle(),
        },
        // Ajoutez les autres pattes ici avec les valeurs des angles aléatoires correspondants
      };
      animatePose(pose, 1000); // 1000ms pour la durée de l'animation (1 seconde)
    });

    function getRandomAngle() {
      return Math.floor(Math.random() * 180);
    }

    // ... (Code inchangé jusqu'à la fin) ...

    function animatePose(targetPose, duration) {
      const initialPose = getCurrentPose();
      const startTime = Date.now();

      function updateAnimation() {
        const currentTime = Date.now();
        const elapsedTime = currentTime - startTime;
        const progress = Math.min(elapsedTime / duration, 1);

        const pose = {};
        for (const leg in targetPose) {
          if (targetPose.hasOwnProperty(leg)) {
            pose[leg] = {};
            for (const angle in targetPose[leg]) {
              if (targetPose[leg].hasOwnProperty(angle)) {
                const initialAngle = initialPose[leg][angle];
                const targetAngle = targetPose[leg][angle];
                const easedAngle = easeInOutQuad(progress) * (targetAngle - initialAngle) + initialAngle;
                pose[leg][angle] = easedAngle;
              }
            }
          }
        }

        drawRobot(pose);

        if (progress < 1) {
          requestAnimationFrame(updateAnimation);
        }
      }

      requestAnimationFrame(updateAnimation);
    }

    function easeInOutQuad(t) {
      return t < 0.5 ? 2 * t * t : -1 + (4 - 2 * t) * t;
    }

    function getCurrentPose() {
      // Fonction pour obtenir la pose actuelle du robot
      // Vous devrez remplacer ceci par la logique pour récupérer les angles actuels des servomoteurs
      // Par exemple, vous pouvez obtenir les valeurs actuelles des sliders.
      return {
        leftFront: {
          alpha: sliderValues.alpha || 90,
          beta: sliderValues.beta || 100,
          gamma: sliderValues.gamma || 90,
        },
        rightFront: {
          alpha: sliderValues.alpha || 90,
          beta: sliderValues.beta || 100,
          gamma: sliderValues.gamma || 90,
        },
        // Ajoutez les autres pattes ici
      };
    }