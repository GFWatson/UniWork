using UnityEngine;
using System.Collections;

public class box : MonoBehaviour {

	float x;
	float y;
	public Texture dataBlock;
    bool isVirus = false;
	// Use this for initialization
	void Start () {
		x = Random.Range (0, 400);
		y = Random.Range (0, 200);
        int rnd = Random.Range(1, 10);
        if (rnd == 7)
        {
            isVirus = true;

        }
	}
	
	// Update is called once per frame
	void Update () {
        if (Input.GetMouseButtonDown(0) && isVirus == true)
        {
            Destroy(gameObject);
        }
        else if (Input.GetMouseButtonDown(0) && isVirus == false)
        {
            Destroy(gameObject);

			GUI.DrawTexture(new Rect(x, y, 100, 100), dataBlock, ScaleMode.StretchToFill, true, 0.0f);
        }
	}


}
