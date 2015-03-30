using UnityEngine;
using System.Collections;

public class clickrotate : MonoBehaviour {
public Vector3 RotateAmount = new Vector3(0,300,0);

	void OnMouseDown ()
	{
		if (Input.GetMouseButton (0)) {
			Debug.Log("Button pressed.");
			//while (RotateAmount.y < 360) {
				transform.Rotate(RotateAmount);

			//}
		}
	}
}
